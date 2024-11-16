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
#include <oddf/simulator/common/backend/DataReference.h>

namespace oddf::simulator::common::backend::blocks {

class ProbeAccess : public virtual simulator::backend::IProbeAccess {

public:

	design::NodeType m_nodeType;
	DataReference m_dataReference;

	ProbeAccess(SimulatorBlockOutput const &driver) :
		m_nodeType(driver.GetType()),
		m_dataReference(driver.GetReference())
	{
	}

	virtual void *GetInterface(Uid const &iid) override
	{
		return utility::GetInterfaceHelper<
			simulator::backend::IProbeAccess,
			IObject>::GetInterface(this, iid);
	}

	virtual void Read(void *buffer, size_t size) const override
	{
		m_dataReference.Read(buffer, size);
	}

	virtual size_t GetSize() const noexcept override
	{
		return m_dataReference.GetSize();
	}
};

} // namespace oddf::simulator::common::backend::blocks
