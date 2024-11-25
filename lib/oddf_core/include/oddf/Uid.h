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

    Defines the `Uid` class to represent 128-bit unique identifiers.

*/

#pragma once

#include <cstdint>
#include <tuple>
#include <string>

namespace oddf {

/*
    Represents a 128-bit unique identifier.
*/
class Uid {

private:

	std::tuple<
		std::uint32_t,
		std::uint16_t, std::uint16_t,
		std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t,
		std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t>
		m_data;

public:

	// Constructs from an initialiser list of the form
	// { 0xaaaaaaaa, 0xbbbb, 0xcccc, 0xdd, 0xee, 0xff, 0xgg, 0xhh, 0xii, 0xjj, 0xkk }
	constexpr Uid(std::uint32_t a0, std::uint16_t b0, std::uint16_t b1,
		std::uint8_t c0, std::uint8_t c1, std::uint8_t c2, std::uint8_t c3,
		std::uint8_t c4, std::uint8_t c5, std::uint8_t c6, std::uint8_t c7) :
		m_data(a0, b0, b1, c0, c1, c2, c3, c4, c5, c6, c7)
	{
	}

	constexpr Uid(Uid const &other) :
		m_data(std::get<0>(other.m_data), std::get<1>(other.m_data),
			std::get<2>(other.m_data), std::get<3>(other.m_data),
			std::get<4>(other.m_data), std::get<5>(other.m_data),
			std::get<6>(other.m_data), std::get<7>(other.m_data),
			std::get<8>(other.m_data), std::get<9>(other.m_data),
			std::get<10>(other.m_data))
	{
	}

	constexpr bool operator==(Uid const &other) const
	{
		return m_data == other.m_data;
	}

	constexpr bool operator!=(Uid const &other) const
	{
		return m_data != other.m_data;
	}

	constexpr bool operator<(Uid const &other) const
	{
		return m_data < other.m_data;
	}

	// Returns a string representation of the form {AAAAAAAA-BBBB-CCCC-DDDD-EEEEEEEEEEEE}
	std::string ToString() const;
};

} // namespace oddf
