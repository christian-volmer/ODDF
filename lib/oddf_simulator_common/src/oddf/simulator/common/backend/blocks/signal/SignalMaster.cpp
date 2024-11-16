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

#include "../Signal.h"
#include "SignalAccessObject.h"
#include "I_Signal_Bool.h"

#include <oddf/Exception.h>

namespace oddf::simulator::common::backend::blocks {

SignalMaster::SignalMaster(design::blocks::backend::IDesignBlock const &designBlock) :
	SimulatorBlockBase(designBlock)
{
}

std::string SignalMaster::GetDesignPathHint() const
{
	return GetDesignBlockReference()->GetPath();
}

void SignalMaster::Elaborate(ISimulatorElaborationContext &)
{
	auto outputs = GetOutputsList();

	if (outputs.GetSize() != 1)
		throw Exception(ExceptionCode::Unsupported);

	if (outputs[0].GetType().GetTypeId() != design::NodeType::BOOLEAN)
		throw Exception(ExceptionCode::Unsupported);

	auto inputs = GetInputsList();

	if (inputs.GetSize() != 0)
		throw Exception(ExceptionCode::Unsupported);
}

void SignalMaster::GenerateCode(ISimulatorCodeGenerationContext &context)
{
	auto &signalAccessObject = context.CreateNamedObject<SignalAccessObject>("mysignal",
		context.GetCurrentComponent(),
		GetOutputsList().GetFirst().GetType());

	context.EmitInstruction<I_Signal_Bool>(signalAccessObject);
}

} // namespace oddf::simulator::common::backend::blocks
