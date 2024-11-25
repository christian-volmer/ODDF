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
#include <oddf/simulator/backend/IProbeAccess.h>

#include <type_traits>
#include <cstring>

namespace oddf::simulator::common::backend::blocks {

template<typename simulatorT>
class ProbeAccessObject : public virtual simulator::backend::IProbeAccess {

	/*
	    The current implementation has been tested for `Boolean` and will probably
	    work for `Integer` and `Real`, too. Need to implement correct handling of the
	    dynamic size for bit-vector and fixed-point.

	*/
	static_assert(std::is_same_v<simulatorT, SimulatorType::Boolean>);

public:

	ISimulatorComponent &m_component;
	design::NodeType m_nodeType;
	simulatorT const *m_probedOutputPointer;

	ProbeAccessObject(ISimulatorComponent &component, SimulatorBlockOutput const &driver) :
		m_component(component),
		m_nodeType(driver.GetType()),
		m_probedOutputPointer(driver.GetPointer<simulatorT>())
	{
	}

	ProbeAccessObject(ProbeAccessObject<simulatorT> const &) = delete;
	void operator=(ProbeAccessObject<simulatorT> const &) = delete;

	virtual void *GetInterface(Uid const &iid) override
	{
		return utility::GetInterfaceHelper<
			simulator::backend::IProbeAccess,
			IObject>::GetInterface(this, iid);
	}

	virtual design::NodeType GetType() const noexcept override
	{
		return m_nodeType;
	}

	virtual void Read(void *buffer, size_t count) const override
	{
		if (count != GetSize())
			throw Exception(ExceptionCode::InvalidArgument, "Parameter `count` must be equal to the value returned by `GetSize()`.");

		m_component.EnsureValidState();
		std::memcpy(buffer, m_probedOutputPointer, count);
	}

	virtual size_t GetSize() const noexcept override
	{
		return sizeof(simulatorT);
	}
};

} // namespace oddf::simulator::common::backend::blocks
