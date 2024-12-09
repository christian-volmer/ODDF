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

#include <oddf/utility/CopyBoolean.h>

#include <oddf/Exception.h>

#include <cstdint>

namespace oddf::utility {

void CopyBoolean(void *destination, size_t destinationSize, void const *source, size_t sourceSize)
{
	using uchar = unsigned char;

	uchar const *sourceAsUchar = static_cast<uchar const *>(source);
	uchar *destinationAsUchar = static_cast<uchar *>(destination);

	if (destinationSize < 1)
		throw Exception(ExceptionCode::Overflow);

	bool value = false;

	for (size_t i = 0; i < sourceSize; ++i) {

		if (sourceAsUchar[i] != 0) {

			value = true;
			break;
		}
	}

	destinationAsUchar[0] = value ? 1 : 0;
	for (size_t i = 1; i < destinationSize; ++i)
		destinationAsUchar[i] = 0;
}

} // namespace oddf::utility
