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

#include "backend/ISignalAccess.h"

namespace oddf::simulator {

template<typename accessT>
class Signal;

//
// Signal<bool>
//

template<>
class Signal<bool> {

private:

	backend::ISignalAccess &m_signalAccess;

public:

	Signal(oddf::simulator::ISimulator &simulator, std::string const &name) :
		m_signalAccess(simulator.GetSimulatorAccess().GetNamedObjectInterface<backend::ISignalAccess>(name))
	{
		if (m_signalAccess.GetType().GetTypeId() != design::NodeType::BOOLEAN)
			throw Exception(ExceptionCode::Unsupported);
	}

	void SetValue(bool value)
	{
		std::uint8_t value_uint8 = value ? 1 : 0;
		m_signalAccess.Write(&value_uint8, 1);
	}
};

} // namespace oddf::simulator
