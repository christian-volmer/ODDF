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

#include <type_traits>

namespace oddf::simulator::common::backend {

struct SimulatorInstructionBase;

using SimulatorInstructionFunction = size_t(SimulatorInstructionBase &);

struct SimulatorInstructionBase {

private:

	SimulatorInstructionFunction *m_instructionFunction;

protected:

	template<typename instructionT, std::enable_if_t<std::is_convertible_v<instructionT *, SimulatorInstructionBase *>> * = nullptr>
	SimulatorInstructionBase(size_t (*instructionFunction)(instructionT &)) :
		m_instructionFunction(reinterpret_cast<SimulatorInstructionFunction *>(reinterpret_cast<void *>(instructionFunction)))
	{
	}

public:

	size_t Execute()
	{
		return m_instructionFunction(*this);
	}
};

} // namespace oddf::simulator::common::backend
