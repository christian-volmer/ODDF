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

struct I_Signal_Bool : public SimulatorInstructionBase {

private:

	SignalAccessObject const *m_signalAccessObject;
	SimulatorType::Boolean m_output;

	static size_t InstructionFunction(I_Signal_Bool &instruction)
	{
		instruction.m_output = instruction.m_signalAccessObject->m_value;
		return sizeof(instruction);
	}

public:

	I_Signal_Bool(ISimulatorCodeGenerationContext &context, SignalAccessObject const &signalAccessObject) :
		SimulatorInstructionBase(&InstructionFunction),
		m_signalAccessObject(&signalAccessObject),
		m_output(0)
	{
		context.RegisterOutput(0, m_output);
	}
};

} // namespace oddf::simulator::common::backend::blocks
