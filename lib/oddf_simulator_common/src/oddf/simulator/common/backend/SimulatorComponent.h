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
#include <oddf/simulator/common/backend/ISimulatorComponent.h>

#include <oddf/IObject.h>

#include <memory>
#include <list>
#include <vector>
#include <map>

namespace oddf::simulator::common::backend {

/*
    Class `SimulatorComponent` supports the creation of the component of the
    simulator execution graph. Member `m_blocks` is the list of simulator
    blocks belonging to the component. All components of the execution graph
    are disjoint and can execute independently and therefore in parallel.
*/
class SimulatorComponent : public ISimulatorComponent {

public:

	// Reference to the underlying simulator core.
	SimulatorCore &m_simulatorCore;

	// The bytecode of the component.
	std::vector<char> m_code;

	// Indicates that the component state is invalid. Must not be modified except by members if `SimulatorCore`.
	bool m_invalid;

	// Blocks in this component. Will be cleared once code generation is done.
	std::list<SimulatorBlockBase *> m_blocks;

private:

	std::map<Uid, std::unique_ptr<IObject>> m_componentObjects;

	// Ensures that the state of the component is valid and can safely be accessed.
	virtual void EnsureValidState() override;

	// Marks the state of the component as invalid.
	virtual void InvalidateState() override;

public:

	SimulatorComponent(SimulatorCore &simulatorCore);
	SimulatorComponent(SimulatorComponent const &) = delete;
	void operator=(SimulatorComponent const &) = delete;

	// TODO: the following methods should be moved to class `ComponentBuilder` in BuildComponents.cpp

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

	// Executes the component bytecode.
	void Execute();

	void RegisterComponentObject(Uid const &clsid, std::unique_ptr<IObject> &&object);
	void *GetComponentObject(Uid const &clsid, Uid const &iid) const;
};

} // namespace oddf::simulator::common::backend
