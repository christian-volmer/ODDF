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

#include "../../lib/oddf/src/dfx.h"

#include <oddf/SimulatorCommon.h>
#include <oddf/simulator/Signal.h>
#include <oddf/simulator/Probe.h>

namespace b = dfx::blocks;
namespace sim = oddf::simulator;

int main()
{
	using dfx::dynfix;
	using dfx::ufix;

	//
	// Design
	//

	dfx::Design design;

	/* bool temp = false;
	    b::Probe(!b::Delay(b::Signal(&temp)));*/

	dfx::forward_node<bool> x;
	b::Probe(x);
	x <<= b::Delay(!x);

	//
	// Simulation
	//

	sim::common::Simulator simulator;

	simulator.TranslateDesign(design);

	auto myProbe = sim::Probe<bool>(simulator, "myprobe");
	// auto mySignal = sim::Signal<bool>(simulator, "mysignal");

	simulator.Run(1);
	std::cout << myProbe.GetValue() << "\n";

	simulator.Run(1);
	std::cout << myProbe.GetValue() << "\n";

	simulator.Run(1);
	std::cout << myProbe.GetValue() << "\n";

	// mySignal.SetValue(true);
	std::cout << myProbe.GetValue() << "\n";

	simulator.Run(1);
	std::cout << myProbe.GetValue() << "\n";

	simulator.Run(1);
	std::cout << myProbe.GetValue() << "\n";

	simulator.Run(1);
	std::cout << myProbe.GetValue() << "\n";

	return 0;
}
