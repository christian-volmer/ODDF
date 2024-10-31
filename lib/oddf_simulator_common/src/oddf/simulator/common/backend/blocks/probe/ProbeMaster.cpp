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

#include "../Probe.h"

namespace oddf::simulator::common::backend::blocks {

ProbeMaster::ProbeMaster(design::blocks::backend::IDesignBlock const &designBlock) :
	SimulatorBlockBase(designBlock)
{
}

std::string ProbeMaster::GetDesignPathHint() const
{
	return GetDesignBlockReference()->GetPath();
}

void ProbeMaster::Elaborate(ISimulatorElaborationContext &)
{
}

struct ProbeInstruction : public SimulatorInstructionBase {

private:

	SimulatorType::Boolean const *m_input;

	static size_t InstructionFunction(ProbeInstruction &instruction)
	{
		return sizeof(instruction);
	}

public:

	ProbeInstruction(ISimulatorCodeGenerationContext &context) :
		SimulatorInstructionBase(&InstructionFunction),
		m_input()
	{
		context.RegisterInput(0, m_input);
	}
};

void ProbeMaster::GenerateCode(ISimulatorCodeGenerationContext &context)
{
	context.EmitInstruction<ProbeInstruction>();
}

} // namespace oddf::simulator::common::backend::blocks
