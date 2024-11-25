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

#include <cstddef>

namespace oddf::simulator::common::backend {

class DataReference {

private:

	void const *m_pointer;
	size_t m_size;

public:

	DataReference();
	DataReference(void const *pointer, size_t size);

	size_t GetSize() const noexcept;
	void Read(void *buffer, size_t count) const;
};

} // namespace oddf::simulator::common::backend
