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

#include <oddf/Exception.h>

#include "../SimulatorCore.h"
#include "../SimulatorBlockInternals.h"

#include <cassert>

namespace oddf::simulator::common::backend {

void SimulatorCore::FinaliseTranslation()
{
	class FinalisationContext : public ISimulatorFinalisationContext {

	private:

		// Reference to the component managed by this instance.
		SimulatorComponent &m_component;

		SimulatorBlockBase *m_currentBlock;

		//
		// ISimulatorComponentContext
		//

		virtual void RegisterNamedObject(std::string name, std::unique_ptr<IObject> &&object) override
		{
			m_component.m_simulatorCore.RegisterNamedObject(name, std::move(object));
		}

		virtual ISimulatorComponent &GetCurrentComponent() noexcept override
		{
			return m_component;
		}

	public:

		FinalisationContext(SimulatorComponent &component) :
			m_component(component),
			m_currentBlock()
		{
		}

		FinalisationContext(FinalisationContext const &) = delete;
		void operator=(FinalisationContext const &) = delete;

		void Finalise()
		{
			for (auto *block : m_component.m_blocks) {

				assert(block);
				m_currentBlock = block;
				block->Finalise(*this);
			}

			// We do not need the list of component blocks anymore. Free it.
			decltype(m_component.m_blocks) emptyBlocks;
			std::swap(m_component.m_blocks, emptyBlocks);
		}
	};

	for (auto &component : m_components) {

		auto finalisationContext = FinalisationContext(component);
		finalisationContext.Finalise();

		// Add the component to the set of invalid components.
		assert(component.m_invalid == true);
		m_invalidComponents.insert(&component);
	}

	// We do not need the simulator blocks anymore. Free them.
	decltype(m_blocks) emptyBlocks;
	std::swap(m_blocks, emptyBlocks);
}

} // namespace oddf::simulator::common::backend
