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

#include <oddf/simulator/common/backend/types/GetRequiredByteSize.h>

namespace oddf::simulator::common::backend::types {

size_t GetRequiredByteSize(design::NodeType const &nodeType)
{
	switch (nodeType.GetTypeId()) {

		case design::NodeType::BOOLEAN:
			return 1;

		case design::NodeType::FIXED_POINT:
			return (nodeType.GetWordWidth() + 7) / 8;

		case design::NodeType::INTEGER:
		case design::NodeType::BIT_VECTOR:
		case design::NodeType::REAL:
			throw Exception(ExceptionCode::NotImplemented);
	}

	throw Exception(ExceptionCode::Unexpected);
}

} // namespace oddf::simulator::common::backend::types
