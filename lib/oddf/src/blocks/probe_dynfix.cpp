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

    Implementation of the Probe() block for the common simulator backend.

*/

#include "../global.h"

namespace dfx {
namespace backend {
namespace blocks {

class probe_block_dynfix : public BlockBase {

private:

	InputPin<dynfix> input;

	source_blocks_t GetSourceBlocks() const override
	{
		return source_blocks_t({ input.GetDrivingBlock() });
	}

	bool CanEvaluate() const override
	{
		return false;
	}

	void Evaluate() override
	{
	}

public:

	probe_block_dynfix(node<dynfix> const &theNode) :
		BlockBase("probe"),
		input(this, theNode)
	{
	}

	probe_block_dynfix(probe_block_dynfix const &) = delete;
	void operator=(probe_block_dynfix const &) = delete;
};

} // namespace blocks
} // namespace backend

namespace blocks {

void Probe(node<dynfix> const &theNode)
{
	Design::GetCurrent().NewBlock<backend::blocks::probe_block_dynfix>(theNode);
}

} // namespace blocks
} // namespace dfx
