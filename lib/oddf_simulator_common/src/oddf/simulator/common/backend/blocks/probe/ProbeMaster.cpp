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

#include "../Probe.h"
#include "I_Probe_Bool.h"
#include "ProbeAccess.h"

#include <oddf/Exception.h>

namespace oddf::simulator::common::backend::blocks {

ProbeMaster::ProbeMaster(design::blocks::backend::IDesignBlock const &designBlock) :
	SimulatorBlockBase(designBlock)
{
}

std::string ProbeMaster::GetDesignPathHint() const
{
	return GetDesignBlockReference()->GetPath();
}

void ProbeMaster::Elaborate(ISimulatorElaborationContext &)
{
	auto outputs = GetOutputsList();

	if (outputs.GetSize() != 0)
		throw Exception(ExceptionCode::Unsupported);

	auto inputs = GetInputsList();

	if (inputs.GetSize() != 1)
		throw Exception(ExceptionCode::Unsupported);

	if (inputs[0].GetType().GetTypeId() != design::NodeType::BOOLEAN)
		throw Exception(ExceptionCode::Unsupported);
}

void ProbeMaster::GenerateCode(ISimulatorCodeGenerationContext &context)
{
	context.EmitInstruction<I_Probe_Bool>();
}

void ProbeMaster::Finalise(ISimulatorFinalisationContext &context)
{
	auto &driver = GetInputsList()[0].GetDriver();

	context.CreateNamedObject<ProbeAccess>("myprobe", driver);
}

} // namespace oddf::simulator::common::backend::blocks
