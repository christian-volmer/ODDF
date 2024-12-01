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

#include <oddf/utility/GetInterfaceHelper.h>
#include <oddf/simulator/backend/ISignalAccess.h>
#include <oddf/simulator/common/backend/ISimulatorComponent.h>

#include <cstring>

namespace oddf::simulator::common::backend::blocks {

class SignalAccessObject : public virtual simulator::backend::ISignalAccess {

public:

	ISimulatorComponent &m_component;
	design::NodeType m_nodeType;
	types::Boolean::ValueType m_value;

	SignalAccessObject(ISimulatorComponent &component, design::NodeType const &nodeType) :
		m_component(component),
		m_nodeType(nodeType),
		m_value()
	{
	}

	virtual void *GetInterface(Uid const &iid) override
	{
		return utility::GetInterfaceHelper<
			simulator::backend::ISignalAccess,
			IObject>::GetInterface(this, iid);
	}

	virtual design::NodeType GetType() const noexcept override
	{
		return m_nodeType;
	}

	virtual void Write(void const *buffer, size_t count) override
	{
		if (count > 1)
			throw Exception(ExceptionCode::Bounds);

		if (count == 1) {

			std::uint8_t newValue = 0;

			std::memcpy(&newValue, buffer, 1);

			newValue = newValue != 0;

			if (newValue != m_value) {

				m_value = newValue;
				m_component.InvalidateState();
			}
		}
	}

	virtual size_t GetSize() const noexcept override
	{
		return 1;
	}
};

} // namespace oddf::simulator::common::backend::blocks
