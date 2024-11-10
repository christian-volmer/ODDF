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

#include "../SimulatorCore.h"

#include <oddf/Exception.h>

namespace oddf::simulator::common::backend {

void *SimulatorCore::GetNamedObjectInterface(std::string const &name, Uid const &iid) const
{
	auto objectIt = m_namedSimulatorObjects.find(name);

	if (objectIt == m_namedSimulatorObjects.end())
		throw Exception(ExceptionCode::NoResource);
	else
		return objectIt->second.get()->GetInterface(iid);
}

} // namespace oddf::simulator::common::backend
