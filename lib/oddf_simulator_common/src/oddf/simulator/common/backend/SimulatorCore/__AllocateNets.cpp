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

#include "../SimulatorCore.h"
#include "../SimulatorBlockInternals.h"

#include <oddf/design/NodeType.h>
#include <oddf/Exception.h>

#include <iostream>
#include <cassert>
#include <utility>

namespace oddf::simulator::common::backend {

void SimulatorCore::AllocateNets()
{
	/*
	    AllocateNets
	    ------------

	    Dies geschieht automatisch, die Blöcke selbst werden nicht involviert.

	    Zunächst werden keine Adressen vergeben, da nicht bekannt ist, wie viele
	    man braucht. Das läuft erst über Indizes, diese beschreiben einen
	    Byte-Offset ab dem Beginn des Speicherblockes, der dann am Schluss
	    reserviert wird. Abschließend werden alle Blöcke nochmals durchgegangen
	    und die Indizes durch Addressen ersetzt.

	    Addressen müssen aligned sein. Wenn Adressen übersprungen werden müssen,
	    so werden diese als freie Adressen in anderen Listen vermerkt.

	    Zum Beispiel: Es wurde ein Byte reserviert und dann ein Double. Die sieben
	    freien Plätze werden vermerkt als
	        1 Byte mit Alignment 1, 1 Wort mit Alignnent 2 und 1 Dopplewort mit
	        Alignment 4

	    Referenzzählung implementieren --> Addressen wiederverwenden.


	    SimulatorOutput::DeclareExclusiveAccess() --> Es wird eine exklusive
	    Addresse für den Output reserviert, sodass auch außerhalb des normelen
	    Execution-Flows Werte dort abgefragt bzw. gesetzt werden können.
	     - Auch für Konstanten verwendbar?

	*/

	std::cout << "\n";
	std::cout << "--- Allocating nets ---\n";
	std::cout << "\n";

	class ComponentNetAllocator {

	private:

		// Reference to the component managed by this instance.
		SimulatorComponent &m_component;

		// Index of the next available net address.
		size_t m_nextNetIndex;

		std::pair<size_t, size_t> SizeAndAlignmentForNodeType(design::NodeType const &nodeType)
		{
			using TypeId = design::NodeType::TypeId;

			switch (nodeType.GetTypeId()) {

				case TypeId::BOOLEAN:
					return std::make_pair(sizeof(bool), alignof(bool));
			}

			throw Exception(ExceptionCode::Unexpected, "Type '" + nodeType.ToString() + "' cannot be handled by the simulator.");
		}

		// Returns an index of an available net address either by allocating a
		// new address or by returning one that has been release by a call to
		// `ReleaseNetIndex()`.
		size_t AllocateNetIndex(design::NodeType const &nodeType)
		{
			size_t size, alignment;
			std::tie(size, alignment) = SizeAndAlignmentForNodeType(nodeType);

			size_t skip = (alignment - (m_nextNetIndex % alignment)) % alignment;

			// TODO: remember the skipped addresses. See comment inside `ReleaseNetIndex()`.

			m_nextNetIndex += skip;

			assert(m_nextNetIndex % alignment == 0);

			size_t returnNetIndex = m_nextNetIndex;

			m_nextNetIndex += size;

			return returnNetIndex;
		}

		void ReleaseNetIndex(size_t /* address */, design::NodeType const & /* nodeType */)
		{
			/*
			    TODO: Put the address and and the size in a array sorted by
				size. `AllocateNetIndex()` goes through the array until it
				finds an address of appropriate size and alignment.

			    TODO: remember the skipped addresses above.
			*/
		}

		void AcquireOutput(SimulatorBlockOutput &output)
		{
			assert(output.m_netIndex == SimulatorBlockOutput::EmptyNetIndex);

			output.m_netRefCount += output.GetTargetsCollection().GetSize();
			output.m_netIndex = AllocateNetIndex(output.GetType());
		}

		void ReleaseOutput(SimulatorBlockOutput &output)
		{
			assert(output.m_netRefCount);
			assert(output.m_netIndex != SimulatorBlockOutput::EmptyNetIndex);

			if (--output.m_netRefCount == 0)
				ReleaseNetIndex(output.m_netIndex, output.GetType());
		}

		// Calls `AcquireOutput()` on all outputs of `block`.
		void AcquireBlockOutputs(SimulatorBlockBase &block)
		{
			for (auto &output : block.m_internals->m_outputs)
				AcquireOutput(output);
		}

		// Calls `ReleaseOutput()` on outputs driving `block`.
		void ReleaseBlockInputs(SimulatorBlockBase &block)
		{
			for (auto &input : block.m_internals->m_inputs)
				if (input.IsConnected())
					ReleaseOutput(input.GetDriver());
		}

		// Converts `m_netIndex` to `m_netAddress`.
		void SetBlockNetAddresses(SimulatorBlockBase &block, char *netsBase)
		{
			for (auto &output : block.m_internals->m_outputs)
				output.m_netAddress = netsBase + output.m_netIndex;
		}

	public:

		ComponentNetAllocator(SimulatorComponent &component) :
			m_component(component),
			m_nextNetIndex()
		{
		}

		void AllocateNets()
		{
			/*
			    Addresses becomes allocated by going to the blocks in the order
			    of their execution. Nets are managed by the output that drives
			    the net. Reference counting is implemented so addresses can be
			    reused.
			*/

			for (auto *block : m_component.m_blocks) {

				assert(block);

				/*
				    First, the reference counts of all outputs consumed by this
				    block's inputs become decremented.
				*/

				ReleaseBlockInputs(*block);

				/*
				    Then, new addresses becomes allocated on all outputs of the
				    block. Addresses released by the previous call to
				    `ReleaseBlockInputs()` may become reassigned to this block's
				    outputs.

				    IMPORTANT: Therefore, during execution, outputs must not be
				    written to unless all input data has been processed and can
				    be safely overwritten.
				*/

				AcquireBlockOutputs(*block);
			}

			/*
				The previous loop collected the actual amount of memory
				required. Now we assign the actual addresses.
			*/

			m_component.m_netsBase.reset(new char[m_nextNetIndex]);
			for (auto *block : m_component.m_blocks)
				SetBlockNetAddresses(*block, m_component.m_netsBase.get());
		}
	};

	/*

	    Every component has its own contiguous block of memory used for
	    storing the values on the nets on the component during code execution.

	*/

	for (auto &component : m_components) {

		auto componentNetAllocator = ComponentNetAllocator(component);
		componentNetAllocator.AllocateNets();
	}
}

} // namespace oddf::simulator::common::backend
