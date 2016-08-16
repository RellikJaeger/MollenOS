/* MollenOS
*
* Copyright 2011 - 2016, Philip Meulengracht
*
* This program is free software : you can redistribute it and / or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation ? , either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.If not, see <http://www.gnu.org/licenses/>.
*
*
* MollenOS MCORE - Timer Manager
*/

/* Includes */
#include <DeviceManager.h>
#include <Devices\Timer.h>
#include <Arch.h>
#include <Timers.h>
#include <Scheduler.h>
#include <Heap.h>

/* C-Library */
#include <stddef.h>
#include <ds/list.h>

/* Globals */
List_t *GlbTimers = NULL;
TimerId_t GlbTimerIds = 0;
int GlbTimersInitialized = 0;

/* Init */
void TimersInit(void)
{
	/* Create list */
	GlbTimers = ListCreate(KeyInteger, LIST_SAFE);
	GlbTimersInitialized = 1;
	GlbTimerIds = 0;
}

TimerId_t TimersCreateTimer(TimerHandler_t Callback,
	void *Args, MCoreTimerType_t Type, size_t Timeout)
{
	/* Variables */
	MCoreTimer_t *TimerInfo;
	DataKey_t Key;
	TimerId_t Id;

	/* Sanity */
	if (GlbTimersInitialized != 1)
		TimersInit();

	/* Allocate */
	TimerInfo = (MCoreTimer_t*)kmalloc(sizeof(MCoreTimer_t));
	TimerInfo->Callback = Callback;
	TimerInfo->Args = Args;
	TimerInfo->Type = Type;
	TimerInfo->PeriodicMs = Timeout;
	TimerInfo->MsLeft = (ssize_t)Timeout;

	/* Append to list */
	Key.Value = (int)GlbTimerIds;
	ListAppend(GlbTimers, ListCreateNode(Key, Key, TimerInfo));

	/* Increase */
	Id = GlbTimerIds;
	GlbTimerIds++;

	/* Done */
	return Id;
}

/* Destroys and removes a timer */
void TimersDestroyTimer(TimerId_t TimerId)
{
	/* Variables */
	MCoreTimer_t *Timer = NULL;
	DataKey_t Key;

	/* Get Node */
	Key.Value = (int)TimerId;
	Timer = (MCoreTimer_t*)ListGetDataByKey(GlbTimers, Key, 0);

	/* Sanity */
	if (Timer == NULL)
		return;

	/* Remove By Id */
	ListRemoveByKey(GlbTimers, Key);

	/* Free */
	kfree(Timer);
}

/* Sleep function */
void SleepMs(size_t MilliSeconds)
{
	/* Lookup */
	MCoreDevice_t *tDevice = DmGetDevice(DeviceTimer);

	/* Sanity - 
	 * we make sure there is a timer present 
	 * in the system, otherwise we must induce delay */
	if (tDevice == NULL) {
		DelayMs(MilliSeconds);
		return;
	}

	/* Enter sleep */
	SchedulerSleepThread(NULL, MilliSeconds);
}

/* Stall functions */
void StallMs(size_t MilliSeconds)
{
	/* Lookup */
	MCoreDevice_t *tDevice = DmGetDevice(DeviceTimer);
	MCoreTimerDevice_t *Timer = NULL;

	/* Sanity */
	if (tDevice == NULL)
	{
		DelayMs(MilliSeconds);
		return;
	}

	/* Cast */
	Timer = (MCoreTimerDevice_t*)tDevice->Data;

	/* Go */
	Timer->Stall(tDevice, MilliSeconds);
}

void StallNs(size_t NanoSeconds)
{
	/* Lookup */
	MCoreDevice_t *tDevice = DmGetDevice(DevicePerfTimer);
	MCoreTimerDevice_t *Timer = NULL;

	/* Sanity */
	if (tDevice == NULL)
	{
		DelayMs((NanoSeconds / 1000) + 1);
		return;
	}

	/* Cast */
	Timer = (MCoreTimerDevice_t*)tDevice->Data;

	/* Go */
	Timer->Stall(tDevice, NanoSeconds);
}

/* This should be called by only ONE periodic irq */
void TimersApplyMs(size_t Ms)
{
	ListNode_t *i;

	/* Sanity */
	if (GlbTimersInitialized != 1)
		return;

	/* Apply time to scheduler */
	SchedulerApplyMs(Ms);

	/* Now iterate */
	_foreach(i, GlbTimers)
	{
		/* Cast */
		MCoreTimer_t *Timer = (MCoreTimer_t*)i->Data;

		/* Decreament */
		Timer->MsLeft -= Ms;

		/* Pop timer? */
		if (Timer->MsLeft <= 0)
		{
			/* Yay! Pop! */
			ThreadingCreateThread("Timer Callback", Timer->Callback, Timer->Args, 0);

			/* Restart? */
			if (Timer->Type == TimerPeriodic)
				Timer->MsLeft = (ssize_t)Timer->PeriodicMs;
			else
			{
				/* Remove */
				ListRemoveByNode(GlbTimers, i);

				/* Free timer */
				kfree(Timer);

				/* Free node */
				kfree(i);
			}
		}
	}
}