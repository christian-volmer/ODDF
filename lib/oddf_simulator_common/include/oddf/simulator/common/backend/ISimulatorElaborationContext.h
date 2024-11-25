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

#include <memory>

namespace oddf::simulator::common::backend {

class SimulatorBlockBase;

class ISimulatorElaborationContext {

public:

	virtual ~ISimulatorElaborationContext() { }

	virtual void AddSimulatorBlock(std::unique_ptr<SimulatorBlockBase> &&block) = 0;
	virtual void RemoveThisBlock() = 0;

	template<typename T, typename... argsTs>
	T &AddSimulatorBlock(argsTs &&...args)
	{
		auto uptr = std::make_unique<T>(std::forward<argsTs>(args)...);
		T *p = uptr.get();
		AddSimulatorBlock(std::move(uptr));
		return *p;
	}
};

} // namespace oddf::simulator::common::backend
