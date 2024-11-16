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
#include <oddf/IObject.h>

namespace oddf::design::blocks::backend {

class IConstantBlock : public virtual IObject {

public:

	static constexpr Uid IID = { 0xde952010, 0x8672, 0x4886, 0x98, 0x33, 0xdb, 0xdc, 0x76, 0x3, 0xdb, 0x5b };

	virtual ~IConstantBlock() { }

	virtual void Read(void *buffer, size_t count) const = 0;
	virtual size_t GetSize() const noexcept = 0;
};

} // namespace oddf::design::blocks::backend
