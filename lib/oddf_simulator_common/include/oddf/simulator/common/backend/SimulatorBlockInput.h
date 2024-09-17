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

namespace oddf::simulator::common::backend {

class SimulatorCore;
class SimulatorBlockOutput;
class SimulatorBlockBase;

class SimulatorBlockInput {

private:

	friend SimulatorCore;
	friend SimulatorBlockOutput;
	friend SimulatorBlockBase;

	SimulatorBlockBase &m_owningBlock;
	SimulatorBlockOutput *m_driver;
	size_t m_index;

public:

	SimulatorBlockInput(SimulatorBlockBase &owningBlock, size_t index);
	SimulatorBlockInput(SimulatorBlockInput const &) = delete;
	SimulatorBlockInput(SimulatorBlockInput &&);

	// Returns the index of this output within the list of outputs of the owning block.
	size_t GetIndex() const;

	// Returns the simulator block that owns this input.
	SimulatorBlockBase const &GetOwningBlock() const;

	// Returns the simulator block that owns this input.
	SimulatorBlockBase &GetOwningBlock();

	// Returns whether this inupt is connected to a driving output of another block.
	bool IsConnected() const;

	// If connected, returns a reference to the output of the driving block. Throws otherwise.
	SimulatorBlockOutput const &GetDriver() const;

	// If connected, returns a reference to the output of the driving block. Throws otherwise.
	SimulatorBlockOutput &GetDriver();

	// If not connected, connects this input to the given output. Throws if already connected.
	void ConnectTo(SimulatorBlockOutput &output);

	// If connected, disconnects the input from the driving output. Throws if unconnected.
	void Disconnect();
};

} // namespace oddf::simulator::common::backend
