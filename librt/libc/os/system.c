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
 * MollenOS System Interface
 */

/* Includes */
#include <os/mollenos.h>
#include <os/syscall.h>
#include <os/utils.h>

/* Includes
 * - C-Library */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/* Const Message */
const char *__SysTypeMessage = "LIBC";

/* SystemDebug 
 * Debug/trace printing for userspace application and drivers */
void
SystemDebug(
	_In_ int Type,
	_In_ __CONST char *Format, ...)
{
	// Static storage
	va_list Args;
	char TmpBuffer[256];

	// Reset buffer
	memset(&TmpBuffer[0], 0, sizeof(TmpBuffer));

	// Now use that one to format the string
	// in using sprintf
	va_start(Args, Format);
	vsprintf(&TmpBuffer[0], Format, Args);
	va_end(Args);

	// Now spit it out
    Syscall_Debug(Type, __SysTypeMessage, &TmpBuffer[0]);
}

/* End Boot Sequence */
void MollenOSEndBoot(void) {
    Syscall_SystemStart();
}

/* ScreenQueryGeometry
 * This function returns screen geomemtry
 * descriped as a rectangle structure */
OsStatus_t 
ScreenQueryGeometry(
	_Out_ Rect_t *Rectangle)
{
	/* Vars */
	OSVideoDescriptor_t VidDescriptor = { 0 };

	/* Do it */
	// TODO
	//MollenOSDeviceQuery(DeviceVideo, 0, &VidDescriptor, sizeof(OSVideoDescriptor_t));

	/* Save info */
	Rectangle->x = 0;
	Rectangle->y = 0;
	Rectangle->w = VidDescriptor.Width;
	Rectangle->h = VidDescriptor.Height;

	return OsSuccess;
}

/* SystemTime
 * Retrieves the system time. This is only ticking
 * if a system clock has been initialized. */
OsStatus_t
SystemTime(
	_Out_ struct tm *time) {
    return Syscall_SystemTime(time);
}

/* SystemTick
 * Retrieves the system tick counter. This is only ticking
 * if a system timer has been initialized. */
OsStatus_t
SystemTick(
	_Out_ clock_t *clock) {
    return Syscall_SystemTick(clock);
}

/* QueryPerformanceFrequency
 * Returns how often the performance timer fires every
 * second, the value will never be 0 */
OsStatus_t
QueryPerformanceFrequency(
	_Out_ LargeInteger_t *Frequency) {
    return Syscall_SystemPerformanceFrequency(Frequency);
}

/* QueryPerformanceTimer 
 * Queries the created performance timer and returns the
 * information in the given structure */
OsStatus_t
QueryPerformanceTimer(
	_Out_ LargeInteger_t *Value) {
    return Syscall_SystemPerformanceTime(Value);
}

/* FlushHardwareCache
 * Flushes the specified hardware cache. Should be used with caution as it might
 * result in performance drops. */
OsStatus_t
FlushHardwareCache(
    _In_     int    Cache,
    _In_Opt_ void*  Start, 
    _In_Opt_ size_t Length) {
    return Syscall_FlushHardwareCache(Cache, Start, Length);
}