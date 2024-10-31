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

namespace b = dfx::blocks;

template<typename T = void>
class ProbeValue {

private:

	oddf::simulator::backend::ISimulatorAccess &m_simulator;

public:

	ProbeValue(oddf::simulator::ISimulator &simulator, std::string const &) :
		m_simulator(simulator.GetSimulatorAccess())
	{
	}

	template<typename S = T>
	S get()
	{
		return {};
	}
};

int main()
{
	using dfx::dynfix;
	using dfx::ufix;

	dfx::Design design;

#if 0

	using type = ufix<16>;
	dfx::forward_node<type> current_value;

	// The starting value of our counter
	type starting_value = 0;

	// The increment of our counter
	type increment = 1;

	type stopping_value = 10;

	// The following creates a reset pulse at the beginning of the simulation
	dfx::node<bool> reset = !b::Delay(b::Constant(true));

	{
		DFX_INSTANCE("I_counter", "Counter");

		DFX_INPUT(reset);

		current_value <<= b::Decide(
			reset, starting_value,
			b::Delay(b::Decide(
				current_value < stopping_value, b::FloorCast<type>(current_value + increment),
				current_value)));

		DFX_OUTPUT(current_value);
	}

	dfx::debug::Logger.Log("reset", reset);
	dfx::debug::Logger.Log("out", current_value);

#endif

	b::Probe(!b::Constant(true));

	/*
	    dfx::Simulator simulator(design);
	    simulator.Run(15);
	    dfx::debug::Logger.WriteTable(std::cout);
	*/

	oddf::simulator::common::Simulator simulator2;

	simulator2.TranslateDesign(design);

	simulator2.GetSimulatorAccess().EnsureValid();

	auto probeValue = ProbeValue<bool>(simulator2, "test");

	std::cout << probeValue.get() << "\n";
	// simulator2.Run(1);

	std::cout << probeValue.get<bool>() << "\n";
	// simulator2.Run(1);

	std::cout << probeValue.get<bool>() << "\n";
	// simulator2.Run(1);

	std::cout << probeValue.get<bool>() << "\n";
	// simulator2.Run(1);

	return 0;
}
