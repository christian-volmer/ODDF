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

    <no description>

*/

#pragma once

#include <string>
#include <utility>

namespace oddf {

class ResourcePath : public std::string {

public:

	using std::string::string;

	ResourcePath(std::string const &str) :
		std::string(str) { }

	ResourcePath(std::string &&str) :
		std::string(std::move(str)) { }

	std::string ToString() const
	{
		return *this;
	}
};

} // namespace oddf
