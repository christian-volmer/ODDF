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

#include "ProbeAccessObject.h"

#include <oddf/simulator/common/backend/Types.h>

#include <oddf/utility/GetInterfaceHelper.h>
#include <oddf/utility/CopyBoolean.h>
#include <oddf/utility/CopyInteger.h>

#include <type_traits>

namespace oddf::simulator::common::backend::blocks {

template<typename simulatorT>
void *ProbeAccessObject<simulatorT>::GetInterface(Uid const &iid)
{
	return utility::GetInterfaceHelper<
		simulator::backend::IProbeAccess,
		IObject>::GetInterface(this, iid);
}

template<typename simulatorT>
design::NodeType ProbeAccessObject<simulatorT>::GetType() const noexcept
{
	return m_nodeType;
}

template<typename simulatorT>
size_t ProbeAccessObject<simulatorT>::GetSize() const noexcept
{
	return types::GetRequiredByteSize(m_nodeType);
}

//
// types::Boolean
//

template void *ProbeAccessObject<types::Boolean>::GetInterface(Uid const &iid);
template design::NodeType ProbeAccessObject<types::Boolean>::GetType() const noexcept;
template size_t ProbeAccessObject<types::Boolean>::GetSize() const noexcept;

template<>
void ProbeAccessObject<types::Boolean>::Read(void *buffer, size_t count) const
{
	m_component.EnsureValidState();
	utility::CopyBoolean(buffer, count, m_probedOutputPointer, types::GetStoredByteSize(m_nodeType));
}

//
// types::FixedPointElement
//

template void *ProbeAccessObject<types::FixedPointElement>::GetInterface(Uid const &iid);
template design::NodeType ProbeAccessObject<types::FixedPointElement>::GetType() const noexcept;
template size_t ProbeAccessObject<types::FixedPointElement>::GetSize() const noexcept;

template<>
void ProbeAccessObject<types::FixedPointElement>::Read(void *buffer, size_t count) const
{
	m_component.EnsureValidState();
	utility::CopyUnsignedInteger(buffer, count, m_probedOutputPointer, types::GetStoredByteSize(m_nodeType));
}

} // namespace oddf::simulator::common::backend::blocks
