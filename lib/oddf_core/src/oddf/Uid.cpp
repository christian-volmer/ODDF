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

    Implements the `Uid` class.

*/

#include <oddf/Uid.h>

#include <cinttypes>
#include <cstdio>

namespace oddf {

std::string Uid::ToString() const
{
	std::string uidString;

	uidString.resize(38);

	std::snprintf(uidString.data(), uidString.capacity() + 1, "{%08" PRIx32 "-%04" PRIx16 "-%04" PRIx16 "-%02" PRIx8 "%02" PRIx8 "-%02" PRIx8 "%02" PRIx8 "%02" PRIx8 "%02" PRIx8 "%02" PRIx8 "%02" PRIx8 "}",
		std::get<0>(m_data),
		std::get<1>(m_data), std::get<2>(m_data),
		std::get<3>(m_data), std::get<4>(m_data), std::get<5>(m_data), std::get<6>(m_data), std::get<7>(m_data), std::get<8>(m_data), std::get<9>(m_data), std::get<10>(m_data));

	return uidString;
}

} // namespace oddf
