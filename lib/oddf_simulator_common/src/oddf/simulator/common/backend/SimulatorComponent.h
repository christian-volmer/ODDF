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

#include <oddf/simulator/common/backend/SimulatorBlockBase.h>

#include <list>
#include <vector>

namespace oddf::simulator::common::backend {

/*
    Class `SimulatorComponent` supports the creation of the component of the
    simulator execution graph. Member `m_blocks` is the list of simulator
    blocks belonging to the component. All components of the execution graph
    are disjoint and can execute independently and therefore in parallel.
*/
class SimulatorComponent {

public:

	// Blocks in this component.
	std::list<SimulatorBlockBase *> m_blocks;

	std::vector<char> m_code;

	// Base pointer to the storage for the component's nets.
	// TODO remove? std::unique_ptr<char[]> m_netsBase;

	SimulatorComponent();
	SimulatorComponent(SimulatorComponent const &) = delete;
	void operator=(SimulatorComponent const &) = delete;

	// Adds `block` to the current component.
	void AddBlock(SimulatorBlockBase &block);

	// Moves blocks from `other` into the component by appending them at the end
	// of the list of blocks. After the call `other` will be an empty component.
	void MoveAppendFromOther(SimulatorComponent *other);

	// Moves blocks from `other` into the component by prepending them at the
	// beginning of the list of blocks. After the call `other` will be an empty
	// component.
	void MovePrependFromOther(SimulatorComponent *other);

	// Returns whether the component is empty.
	bool IsEmpty() const;

	// Returns the number of blocks in this component.
	size_t GetSize() const;

	void EnsureValid();
};

} // namespace oddf::simulator::common::backend
