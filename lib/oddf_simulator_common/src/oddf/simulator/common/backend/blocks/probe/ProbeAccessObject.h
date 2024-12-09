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

#include <oddf/utility/CopyBoolean.h>
#include <oddf/utility/CopyInteger.h>

#include <oddf/simulator/common/backend/Types.h>

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
	static_assert(std::is_same_v<simulatorT, types::Boolean>
		|| std::is_same_v<simulatorT, types::FixedPointElement>);

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

	ProbeAccessObject(ISimulatorComponent &component, SimulatorBlockOutput const &driver, size_t elementCount) :
		m_component(component),
		m_nodeType(driver.GetType()),
		m_probedOutputPointer(driver.GetPointer<simulatorT>(elementCount))
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

	virtual void Read(void *buffer, size_t count) const override;

	virtual size_t GetSize() const noexcept override
	{
		return types::GetRequiredByteSize(m_nodeType);
	}
};

template<>
inline void ProbeAccessObject<types::Boolean>::Read(void *buffer, size_t count) const
{
	m_component.EnsureValidState();
	utility::CopyBoolean(buffer, count, m_probedOutputPointer, types::GetStoredByteSize(m_nodeType));
}

template<>
inline void ProbeAccessObject<types::FixedPointElement>::Read(void *buffer, size_t count) const
{
	m_component.EnsureValidState();
	utility::CopyUnsignedInteger(buffer, count, m_probedOutputPointer, types::GetStoredByteSize(m_nodeType));
}

} // namespace oddf::simulator::common::backend::blocks
