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

#include "../Not.h"

#include <oddf/simulator/common/backend/SimulatorTypes.h>

namespace oddf::simulator::common::backend::blocks {

NotMaster::NotMaster(design::blocks::backend::IDesignBlock const &designBlock) :
	SimulatorBlockBase(designBlock)
{
}

std::string NotMaster::GetDesignPathHint() const
{
	return GetDesignBlockReference()->GetPath();
}

void NotMaster::Elaborate(ISimulatorElaborationContext &)
{
}

struct NotInstruction : public SimulatorInstructionBase {

private:

	SimulatorType::Boolean const *m_input;
	SimulatorType::Boolean m_output;

	static size_t InstructionFunction(NotInstruction &instruction)
	{
		instruction.m_output = !(*instruction.m_input);
		return sizeof(instruction);
	}

public:

	NotInstruction(ISimulatorCodeGenerationContext &context) :
		SimulatorInstructionBase(&InstructionFunction),
		m_input(), m_output()
	{
		context.RegisterInput(0, m_input);
		context.RegisterOutput(0, m_output);
	}
};

void NotMaster::GenerateCode(ISimulatorCodeGenerationContext &context)
{
	context.EmitInstruction<NotInstruction>();
}

} // namespace oddf::simulator::common::backend::blocks
