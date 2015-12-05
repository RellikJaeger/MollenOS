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
* MollenOS MCore - Shared Memory System
*/

/* Includes */
#include <Arch.h>
#include <Log.h>

/* Shorthand */
#define DefineSyscall(_Sys) ((Addr_t)&_Sys)

/***********************
 * Process Functions   *
 ***********************/
void ScProcessTerminate(int ExitCode)
{
	_CRT_UNUSED(ExitCode);
}

void ScProcessYield(void)
{
	/* Deep Call */
	_ThreadYield();
}

/* NoP */
void NoOperation(void)
{

}

/* Syscall Table */
Addr_t GlbSyscallTable[] =
{
	/* Kernel Log */
	DefineSyscall(LogDebug),

	/* Process Functions */
	DefineSyscall(ScProcessTerminate),
	DefineSyscall(ScProcessYield),

	/* Not Defined */
	DefineSyscall(NoOperation),
	DefineSyscall(NoOperation),
	DefineSyscall(NoOperation),
	DefineSyscall(NoOperation),
	DefineSyscall(NoOperation),
	DefineSyscall(NoOperation)
};