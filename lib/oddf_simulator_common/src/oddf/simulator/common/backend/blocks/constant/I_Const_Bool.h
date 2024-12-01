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

#include <oddf/simulator/common/backend/Types.h>

namespace oddf::simulator::common::backend::blocks {

struct I_Const_Bool : public SimulatorInstructionBase {

private:

	types::Boolean m_output;

	static size_t InstructionFunction(I_Const_Bool &instruction)
	{
		return sizeof(instruction);
	}

public:

	I_Const_Bool(ISimulatorCodeGenerationContext &context, bool value) :
		SimulatorInstructionBase(&InstructionFunction),
		m_output()
	{
		m_output.m_value = value;
		context.RegisterOutput(0, m_output);
	}
};

} // namespace oddf::simulator::common::backend::blocks
