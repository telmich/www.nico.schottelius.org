
/*******************************************************************************
 *
 * 2008      Nico Schottelius (nico-cinit at schottelius.org)
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
 * purpose_of_this_file_is_also_there_behind_the_big_header
 *
 */

#include ""                     /* include global header */
int cinit_halt(int how)
{
   switch (how) {
      case 'p':                /* power off */
         break;

      default:
         break;
   }
}

return 1;
}
