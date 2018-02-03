/* MollenOS
 *
 * Copyright 2011 - 2018, Philip Meulengracht
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
 * MollenOS - C/C++ Test Suite for Userspace
 *  - Runs a variety of userspace tests against the libc/libc++ to verify
 *    the stability and integrity of the operating system.
 */

/* Includes
 * - Tests */
#include "test.hpp"
#include "test_constreams.hpp"
#include <thread>

/*******************************************
 * Tls Testing
 *******************************************/
thread_local std::thread::id thread_id;
void thread_function() {
    thread_id = std::this_thread::get_id();
    TRACE("Thread id of new thread: %u", thread_id);
}

int TestThreading() {
    int ErrorCounter = 0;
    std::thread::id local_id = std::this_thread::get_id();
    thread_id = std::this_thread::get_id();
    TRACE("Thread id of main thread: %u", thread_id);
    std::thread tlsthread(thread_function);
    tlsthread.join();
    TRACE("Thread id of main thread: %u", thread_id);
    if (thread_id != local_id) {
        ErrorCounter++;
    }
    return ErrorCounter;
}

/*******************************************
 * Entry Point
 *******************************************/
int main(int argc, char **argv) {
    int ErrorCounter = 0;

    // Run tests
    RUN_TEST_SUITE(ErrorCounter, ConsoleStreamTests);

    // Run tests that must be in source files
    ErrorCounter += TestThreading();
    return ErrorCounter;
}
