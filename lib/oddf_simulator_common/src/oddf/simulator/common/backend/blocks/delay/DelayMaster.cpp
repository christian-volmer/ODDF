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

#include <oddf/Exception.h>

namespace oddf::simulator::common::backend::blocks {

DelayMaster::DelayMaster(design::blocks::backend::IDesignBlock const &designBlock) :
	SimulatorBlockBase(designBlock)
{
}

std::string DelayMaster::GetDesignPathHint() const
{
	return GetDesignBlockReference()->GetPath();
}

void DelayMaster::Elaborate(ISimulatorElaborationContext &context)
{
	auto outputs = GetOutputsList();

	if (outputs.GetSize() != 1)
		throw Exception(ExceptionCode::Unsupported);

	if (outputs[0].GetType().GetTypeId() != design::NodeType::BOOLEAN)
		throw Exception(ExceptionCode::Unsupported);

	auto inputs = GetInputsList();

	if (inputs.GetSize() != 1)
		throw Exception(ExceptionCode::Unsupported);

	if (inputs[0].GetType().GetTypeId() != design::NodeType::BOOLEAN)
		throw Exception(ExceptionCode::Unsupported);

	/*

	Elaboration splits the original delay block ('DelayMaster') up into two
	separate blocks: a starting point, which acts as the source of the value
	stored in the flip-flop; and an endpoint, which accepts the value for
	storage.

	The following code detaches all original connections from the 'DelayMaster'
	and attaches them to the newly created 'DelayStartingPoint' and
	'DelayEndpoint' simulator blocks.

	*/

	auto &endpoint = context.AddSimulatorBlock<DelayEndpoint>(GetDesignBlockReference());
	auto &startingPoint = context.AddSimulatorBlock<DelayStartingPoint>(GetDesignBlockReference(), design::NodeType::Boolean(), endpoint);

	context.TransferConnectivity(inputs[0], endpoint.GetInputsList()[0]);
	context.TransferConnectivity(outputs[0], startingPoint.GetOutputsList()[0]);

	context.RemoveThisBlock();
}

} // namespace oddf::simulator::common::backend::blocks
