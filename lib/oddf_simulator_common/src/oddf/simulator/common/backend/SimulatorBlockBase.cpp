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

    Implements `SimulatorBaseClass`, which is the base class to all simulator
    blocks.

*/

#include <oddf/simulator/common/backend/SimulatorBlockBase.h>

#include "SimulatorBlockInternals.h"

#include <oddf/Exception.h>

namespace oddf::simulator::common::backend {

SimulatorBlockBase::SimulatorBlockBase(design::blocks::backend::IDesignBlock const &designBlock) :
	m_internals(new Internals(*this, designBlock))
{
}

SimulatorBlockBase::SimulatorBlockBase(size_t numberOfInputs, std::initializer_list<design::NodeType> outputNodeTypes) :
	m_internals(new Internals(*this, numberOfInputs, outputNodeTypes))
{
}

SimulatorBlockBase::~SimulatorBlockBase()
{
}

design::blocks::backend::IDesignBlock const *SimulatorBlockBase::GetDesignBlockReference() const
{
	return m_internals->m_designBlockReference;
}

utility::ListView<SimulatorBlockInput const &> SimulatorBlockBase::GetInputsList() const
{
	return utility::MakeListView<SimulatorBlockInput const &>(m_internals->m_inputs);
}

utility::ListView<SimulatorBlockOutput const &> SimulatorBlockBase::GetOutputsList() const
{
	return utility::MakeListView<SimulatorBlockOutput const &>(m_internals->m_outputs);
}

bool SimulatorBlockBase::HasConnections() const noexcept
{
	for (auto const &input : m_internals->m_inputs)
		if (input.IsConnected())
			return true;

	for (auto const &output : m_internals->m_outputs)
		if (output.HasConnections())
			return true;

	return false;
}

void SimulatorBlockBase::GenerateCode(ISimulatorCodeGenerationContext & /* context */)
{
	// Blocks should either implement this function or remove themselves from the
	// list of simulator blocks during elaboration.
	throw Exception(ExceptionCode::NotImplemented);
}

void SimulatorBlockBase::Finalise(ISimulatorFinalisationContext & /* context */)
{
	// Default implementation does nothing.
}

} // namespace oddf::simulator::common::backend
