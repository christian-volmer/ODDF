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

#include "SimulatorComponent.h"
#include "SimulatorBlockInternals.h"

#include <algorithm>
#include <cassert>

namespace oddf::simulator::common::backend {

SimulatorComponent::SimulatorComponent() :
	m_blocks(),
	m_code()
{
}

void SimulatorComponent::AddBlock(SimulatorBlockBase &block)
{
	assert(!block.m_internals->m_component);
	block.m_internals->m_component = this;
	m_blocks.push_back(&block);
}

void SimulatorComponent::MoveAppendFromOther(SimulatorComponent *other)
{
	assert(other);

	for (auto *block : other->m_blocks) {

		assert(block->m_internals->m_component == other);
		block->m_internals->m_component = this;
	}

	m_blocks.splice(m_blocks.end(), other->m_blocks);

	for (auto *block : m_blocks)
		assert(block->m_internals->m_component == this);

	assert(other->IsEmpty());
}

void SimulatorComponent::MovePrependFromOther(SimulatorComponent *other)
{
	assert(other);

	for (auto *block : other->m_blocks) {

		assert(block->m_internals->m_component == other);
		block->m_internals->m_component = this;
	}

	m_blocks.splice(m_blocks.begin(), other->m_blocks);

	for (auto *block : m_blocks)
		assert(block->m_internals->m_component == this);

	assert(other->IsEmpty());
}

size_t SimulatorComponent::GetSize() const
{
	return m_blocks.size();
}

bool SimulatorComponent::IsEmpty() const
{
	return m_blocks.empty();
}

void SimulatorComponent::EnsureValid()
{
	char *position = m_code.data();
	char *end = m_code.data() + m_code.size();

	while (position < end) {

		auto instruction = reinterpret_cast<SimulatorInstructionBase *>(position);
		position += instruction->Execute();
	}
}

} // namespace oddf::simulator::common::backend
