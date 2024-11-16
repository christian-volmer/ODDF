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

#include "../Constant.h"
#include "I_Const_Bool.h"

#include <oddf/Exception.h>
#include <oddf/design/blocks/backend/IConstantBlock.h>

namespace oddf::simulator::common::backend::blocks {

ConstantMaster::ConstantMaster(design::blocks::backend::IDesignBlock const &designBlock) :
	SimulatorBlockBase(designBlock)
{
}

std::string ConstantMaster::GetDesignPathHint() const
{
	return GetDesignBlockReference()->GetPath();
}

void ConstantMaster::Elaborate(ISimulatorElaborationContext &)
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

void ConstantMaster::GenerateCode(ISimulatorCodeGenerationContext &context)
{

	auto &constantBlock = GetDesignBlockReference()->GetInterface<design::blocks::backend::IConstantBlock>();

	std::uint8_t value = 0;
	constantBlock.Read(&value, 1);

	context.EmitInstruction<I_Const_Bool>(value != 0);
}

} // namespace oddf::simulator::common::backend::blocks
