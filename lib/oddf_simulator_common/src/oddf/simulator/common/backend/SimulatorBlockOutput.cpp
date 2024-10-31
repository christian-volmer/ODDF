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

#include <oddf/simulator/common/backend/SimulatorBlockBase.h>

#include <oddf/Exception.h>

namespace oddf::simulator::common::backend {

SimulatorBlockOutput::SimulatorBlockOutput(SimulatorBlockBase &owningBlock, design::NodeType const &nodeType, size_t index) :
	m_owningBlock(owningBlock),
	m_nodeType(nodeType),
	m_index(index),
	m_targets(),
	m_storageReference()
/*	TODO remove? m_netRefCount(),
    m_netIndex(EmptyNetIndex),
    m_netAddress()*/
{
}

SimulatorBlockOutput::SimulatorBlockOutput(SimulatorBlockOutput &&other) :
	m_owningBlock(other.m_owningBlock),
	m_nodeType(),
	m_index(0),
	m_targets(),
	m_storageReference()
/* TODO remove? m_netRefCount(),
    m_netIndex(EmptyNetIndex),
    m_netAddress()*/
{
	// Since the address of an output object must never change, it must not be moved.
	throw oddf::Exception(oddf::ExceptionCode::Unexpected);
}

design::NodeType SimulatorBlockOutput::GetType() const
{
	return m_nodeType;
}

size_t SimulatorBlockOutput::GetIndex() const
{
	return m_index;
}

SimulatorBlockBase const &SimulatorBlockOutput::GetOwningBlock() const
{
	return m_owningBlock;
}

SimulatorBlockBase &SimulatorBlockOutput::GetOwningBlock()
{
	return m_owningBlock;
}

utility::CollectionView<SimulatorBlockInput const &> SimulatorBlockOutput::GetTargetsCollection() const
{
	return utility::MakeCollectionView<SimulatorBlockInput const &>(m_targets);
}

utility::CollectionView<SimulatorBlockInput &> SimulatorBlockOutput::GetTargetsCollection()
{
	return utility::MakeCollectionView<SimulatorBlockInput &>(m_targets);
}

void SimulatorBlockOutput::DisconnectAll()
{
	auto targets = GetTargetsCollection();

	while (!targets.IsEmpty())
		targets.GetFirst().Disconnect();
}

/* TODO remove?

void SimulatorBlockOutput::DeclareExclusiveAccess()
{
	m_netRefCount = 1;
}

void *SimulatorBlockOutput::GetAddress()
{
	if (!m_netRefCount || !m_netAddress)
		throw oddf::Exception(oddf::ExceptionCode::IllegalMethodCall);

	return m_netAddress;
}

*/

} // namespace oddf::simulator::common::backend
