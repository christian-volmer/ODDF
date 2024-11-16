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

#include "SimulatorCore.h"

#include <cassert>

namespace oddf::simulator::common::backend {

SimulatorCore::SimulatorCore() :
	m_blocks(),
	m_simulatorBlockFactories(),
	m_components(),
	m_invalidComponents(),
	m_namedSimulatorObjects()
{
	RegisterDefaultBlockFactories();
}

SimulatorCore::~SimulatorCore() = default;

void SimulatorCore::InvalidateComponentState(SimulatorComponent &component)
{
	if (!component.m_invalid) {

		component.m_invalid = true;
		m_invalidComponents.insert(&component);
	}
}

void SimulatorCore::EnsureValidComponentState(SimulatorComponent & /* component */)
{
	EnsureAllComponentStatesValid();
}

void SimulatorCore::EnsureAllComponentStatesValid()
{
	for (auto *invalidComponent : m_invalidComponents) {

		assert(invalidComponent->m_invalid);
		invalidComponent->Execute();
		invalidComponent->m_invalid = false;
	}

	m_invalidComponents.clear();
}

} // namespace oddf::simulator::common::backend
