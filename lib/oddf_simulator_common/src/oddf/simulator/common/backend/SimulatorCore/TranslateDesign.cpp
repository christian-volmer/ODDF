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

#include "../SimulatorBlockInternals.h" // TODO: delete if not required anymore.

#include <cassert>
#include <iostream>

namespace oddf::simulator::common::backend {

void SimulatorCore::TranslateDesign(design::IDesign const &design)
{
	std::cout << "\n";
	std::cout << "--- Translating design ---\n";
	std::cout << "\n";

	{
		auto blockMapping = MapBlocks(design);

		MapConnections(*blockMapping);
	}

	ElaborateBlocks();

	BuildComponents();

	GenerateCode();

	size_t componentIndex = 0;

	for (auto &component : m_components) {

		std::cout << "\n-- Component " << componentIndex++ << " --\n\n";

		for (auto const &block : component.m_blocks) {

			if (!block) {

				std::cout << "[block was removed]\n\n";
				continue;
			}

			assert(block->m_internals->m_component == &component);

			std::cout << "<" << block << ">: " << block->GetDesignPathHint() << "\n";

			auto inputsList = block->GetInputsList();

			for (size_t i = 0; i < inputsList.GetSize(); ++i) {

				auto const &input = inputsList[i];

				std::cout << "  input[" << i << "]: ";

				if (input.IsConnected()) {

					auto *drivingBlock = &input.GetDriver().GetOwningBlock();
					std::cout << "driven by <" << drivingBlock << ">: " << drivingBlock->GetDesignPathHint();
				}
				else {

					std::cout << "unconnected";
				}

				std::cout << "\n";
			}

			auto outputsList = block->GetOutputsList();

			for (size_t i = 0; i < outputsList.GetSize(); ++i) {

				auto const &output = outputsList[i];

				std::cout << "  output[" << i << "]: " + outputsList[i].GetType().ToString() << ": ";

				auto targetsCollection = output.GetTargetsCollection();

				if (targetsCollection.GetSize() > 0) {

					std::cout << "driving inputs of";
					auto targetsEnumerator = targetsCollection.GetEnumerator();
					targetsEnumerator.Reset();
					while (targetsEnumerator.MoveNext())
						std::cout << " <" << &targetsEnumerator.GetCurrent().GetOwningBlock() << ">";
				}
				else {

					std::cout << "unconnected";
				}

				std::cout << "\n";
			}

			std::cout << "\n";
		}
	}

	/*

	- Wie melden Blöcke an, dass sie die Knoten-Addresse außerhalb des normalen Execution-Flows benötigen?
	- Wann und wie wird die Größe der Knoten-Daten bestimmt?
	- Wann un dwie werden die Daten-Addressen zugewiesen?
	    - Was machen wir mit verschiedenen Daten-Breiten (int, double, dynfix, bool, bitvector)
	- Wann und wie bekommen Blöcke die Addressen, währen der Code-Generierung?
	- Wann und Wie bekommen Blöcke die Addressen für außerhalb des Execution-Flows
	- Wann und wie werden die Simulator-Objekte erstellt und registriert.
	- Wir brauchen einen Clock-Handler.

	AllocateNets
	    - Speicher für outputs von Blöcken
	    - Speicher für interne knoten wiederverwenden können?
	    - Was ist mit Speicher für delay, memory --> landet in ComponentObjects

	*/

	/*

	GenerateCode
	------------

	Die Blöcke werden in ihrer Ausführungsreihenfolge aufgeforder, eine Struktur
	rauszuschreiben, deren erster Parameter ein Funktionszeiger ist.

	Der genaue Aufbau der Struktur ist Sache des Blocks. Allgemein beinhaltet
	die Struktur

	 - Zeiger zu  Input / Output Values
	    ---> wärend der Code-Generation muss es möglich sein, diese Zeiger
	         zu ermitteln

	 - Weitere ggf. erforderliche Angaben, wie z.B. Anzahl und Datentyp der
	   Operanden.

	Die FUnktion muss die größe der Struktor (sizeof) zurückliefern.

	CodeGenerationContext

	    EmitCode(T *) -> Typ T der Struktur als Template-Parameter
	        übergeben, damit die Struktur mit korrektem Alignment in die
	        Execution-List eingebaut werden kann. Es werden ggf.
	        Füller-Instruktionen emittiert.

	        Rückgabewert muss sizeof(T) sein. (--> was ist mit variabler Struktur?)

	        Die Struktur sollte von einer Basisklasse abgeleitet sein, die
	            - alignas(void *) ist.
	            - Als Member einen void * hat. Womit ersteres vermutlich erfüllt sein dürfte.

	    ISimulatorComponent GetSimulatorComponent()
	        Invalidate(); --> Markiert den Zustand der Komponente als ungültig
	        Evaluate(); --> Evaluiert die entsprechende Komponente.

	    GetComponentObject(std::string const &name, [ factory ]);
	    GetSimulatorObject(ResourcePath const &path, [ factory ]);

	    Wo und wie bringen unter
	    Wie werden die entsprechenden Komponenten invalidiert?
	        Delay
	            - ComponentObject in der Ziel-Komponente.
	                - Für jedes verwaltete Delay:
	                    - Zeiger auf den Endpoint
	                        - Ggf. auch den dazugehörigen clock-enable
	                    - Storage für den gespeicherten Wert
	                - Bei OnClock:
	                    - Kopiert den Wert des Endpoints in die Storage.
	                    - Falls der Wert sich geändert hat, wird die
	                      Komponente als ungültig markiert.

	            - Die StartingPoints haben die Addresse der Storage. Bei
	              Evaluate() kopieren sie die Werte in ihren Ausgangsknoten.

	        Probe
	            - SimulatorObject mit Pfad "/xxx/yyy/probes:UserId".
	            - Kennt Adresse

	        Logging
	        Forcing
	        Assertions / Evaluations
	            --> C++ functions that execute on the values of simulation nodes (Leon equalise bug evaluation)
	            --> Logging could also apply functions to nodes before logging the result
	            --> Nodes must be named in the design (inputs, outputs, labels)
	        Memory
	        Sink
	        Source
	        Signal
	        Random


	    OnClock Ereignis
	    ----------------

	    Vorher werden alle Evaluiert.
	    Danach darf keine Komponente mehr ungültig sein.
	    Es werden alle OnClock-Handler aufgerufen.

	*/
}

} // namespace oddf::simulator::common::backend
