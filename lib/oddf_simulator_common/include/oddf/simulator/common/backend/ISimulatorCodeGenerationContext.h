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

#include "SimulatorInstructionBase.h"

#include "Types.h"

#include "ISimulatorComponentContext.h"

#include <type_traits>
#include <cstdint>

namespace oddf::simulator::common::backend {

class ISimulatorCodeGenerationContext : public virtual ISimulatorComponentContext {

protected:

	virtual void *InternalEmitInstruction(size_t size, size_t alignment) = 0;

public:

	virtual ~ISimulatorCodeGenerationContext() = default;

	template<typename T, typename... argTs>
	void EmitInstruction(argTs &&...args)
	{
		static_assert(std::is_trivially_copyable_v<T>);
		auto *instruction = InternalEmitInstruction(sizeof(T), alignof(T));
		new (instruction) T(*this, std::forward<argTs>(args)...);
	}

	virtual void RegisterInput(size_t index, types::Boolean const *&inputPointerReference) = 0;

	virtual void RegisterOutput(size_t index, types::Boolean &outputReference) = 0;
};

} // namespace oddf::simulator::common::backend
