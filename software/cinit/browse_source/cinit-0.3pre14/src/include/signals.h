/*******************************************************************************
 *
 * 2007-2008 Nico Schottelius (nico-cinit at schottelius.org)
 *
 * This file is part of cinit.

 * cinit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cinit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with cinit.  If not, see <http://www.gnu.org/licenses/>.

 *
 *    Define signals to use: needs signal.h to be included before.
 */

#ifndef CINIT_SIGNALS_HEADER
#define CINIT_SIGNALS_HEADER

#define SIG_CINIT_HALT        SIGUSR1
#define SIG_CINIT_POWEROFF    SIGTERM
#define SIG_CINIT_REBOOT      SIGHUP

#endif