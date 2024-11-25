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
#include <oddf/simulator/common/backend/SimulatorType.h>
#include <oddf/simulator/common/backend/IClockable.h>
#include <oddf/simulator/common/backend/ISimulatorComponent.h>
#include <oddf/simulator/common/backend/DataReference.h>

#include <list>

namespace oddf::simulator::common::backend::blocks {

template<typename T>
struct DelayState;

template<>
struct DelayState<SimulatorType::Boolean> {

	SimulatorType::Boolean const *m_pSource;
	SimulatorType::Boolean m_value;

	DelayState() :
		m_pSource(), m_value(0) { }
};

class DelayObject : public virtual IClockable {

private:

	ISimulatorComponent &m_component;

	std::list<DelayState<SimulatorType::Boolean>> m_booleanStates;

public:

	static constexpr Uid CLSID = { 0x17fb33b1, 0x928f, 0x44af, 0xb9, 0x30, 0xff, 0xec, 0xb8, 0x44, 0x1f, 0xb2 };

	DelayObject(ISimulatorComponent &component) :
		m_component(component),
		m_booleanStates()
	{
	}

	template<typename T>
	DelayState<SimulatorType::Boolean> *AddState();

	//
	// IObject
	//

	virtual void *GetInterface(Uid const &iid) override
	{
		return utility::GetInterfaceHelper<IObject, IClockable>::GetInterface(this, iid);
	}

	//
	// IClockable
	//

	virtual void Clock() override
	{
		for (auto &state : m_booleanStates)
			state.m_value = *state.m_pSource;

		m_component.InvalidateState();
	}
};

template<>
inline DelayState<SimulatorType::Boolean> *DelayObject::AddState<SimulatorType::Boolean>()
{
	return &m_booleanStates.emplace_back();
}

} // namespace oddf::simulator::common::backend::blocks
