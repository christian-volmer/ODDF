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

#include <oddf/simulator/common/backend/SimulatorType.h>

namespace oddf::simulator::common::backend::blocks {

struct I_Probe_Bool : public SimulatorInstructionBase {

private:

	SimulatorType::Boolean const *m_input;

	static size_t InstructionFunction(I_Probe_Bool &instruction)
	{
		// TODO: does a probe have to emit an instruction?
		return sizeof(instruction);
	}

public:

	I_Probe_Bool(ISimulatorCodeGenerationContext &context) :
		SimulatorInstructionBase(&InstructionFunction),
		m_input()
	{
		context.RegisterInput(0, m_input);
	}
};

} // namespace oddf::simulator::common::backend::blocks
