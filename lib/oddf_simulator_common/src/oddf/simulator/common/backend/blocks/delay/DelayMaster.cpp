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

#include "../Delay.h"

namespace oddf::simulator::common::backend::blocks {

DelayMaster::DelayMaster(design::blocks::backend::IDesignBlock const &designBlock) :
	SimulatorBlockBase(designBlock)
{
}

std::string DelayMaster::DebugString() const
{
	return "DelayMaster";
}

void DelayMaster::Elaborate(ISimulatorElaborationContext &context)
{
	/*

	Elaboration splits the original delay block ('DelayMaster') up into two
	separate blocks: a starting point, which acts as the source of the value
	stored in the flip-flop; and an endpoint, which accepts the value for
	storage.

	The following code detaches all original connections from the 'DelayMaster'
	and attaches them to the newly created 'DelayStartingPoint' and
	'DelayEndpoint' simulator blocks.

	*/

	// TODO: check configuration of the delay block (i.e., number of inputs/outputs, their types etc.)

	//
	// The new starting point of the delay
	//

	auto startingPoint = context.AddSimulatorBlock<DelayStartingPoint>();

	auto masterOutputTargets = GetOutputsList().GetFirst().GetTargetsCollection();
	auto &startingPointOutput = startingPoint->GetOutputsList().GetFirst();

	while (!masterOutputTargets.IsEmpty()) {

		auto &outputTarget = masterOutputTargets.GetFirst();
		outputTarget.Disconnect();
		outputTarget.ConnectTo(startingPointOutput);
	}

	//
	// The new endpoint of the delay
	//

	auto endpoint = context.AddSimulatorBlock<DelayEndpoint>();

	auto &masterInput = GetInputsList().GetFirst();

	if (masterInput.IsConnected()) {

		auto &inputDriver = masterInput.GetDriver();
		auto &endpointInput = endpoint->GetInputsList().GetFirst();

		masterInput.Disconnect();
		endpointInput.ConnectTo(inputDriver);
	}

	context.RemoveThisBlock();
}

} // namespace oddf::simulator::common::backend::blocks
