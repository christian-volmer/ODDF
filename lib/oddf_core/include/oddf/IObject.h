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

#include <oddf/Uid.h>

namespace oddf {

class IObject {

public:

	static constexpr Uid IID = { 0x8a54167e, 0x6e89, 0x4a1a, 0xb2, 0xd5, 0xfc, 0xa9, 0xaa, 0xb, 0xe6, 0xb };

	virtual ~IObject() { }

	virtual void *GetInterface(Uid const &iid) = 0;

	template<typename T>
	T &GetInterface()
	{
		return *static_cast<T *>(GetInterface(T::IID));
	}
};

} // namespace oddf
