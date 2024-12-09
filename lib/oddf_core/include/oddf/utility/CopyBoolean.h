/*

    ODDF - Open Digital Design Framework
    Copyright Advantest Corporation

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

/*

    Function for copying data from one memory location to another under
    the assumption that the data represents a boolean value.

*/

#pragma once

#include <cstddef>

namespace oddf::utility {

// Copies a boolean value from the memory region of size `sourceSize` pointed
// to by `source` to the memory region of size `destinationSize` pointed to by
// `destination`. A non-zero byte anywhere within the source memory region
// signifies the value `true`. The first byte in the destination region is set
// to 0x01 to represent the value `true` and set to 0x00 to represent the value
// `false`. All but the first destination bytes are set to 0x00.
void CopyBoolean(void *destination, size_t destinationSize, void const *source, size_t sourceSize);

} // namespace oddf::utility
