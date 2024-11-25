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

#include "backend/ISimulatorBlockFactory.h"

#include <oddf/design/IDesign.h>
#include <oddf/simulator/ISimulator.h>
#include <oddf/design/blocks/backend/DesignBlockClass.h>

#include <memory>

namespace oddf::simulator::common {

namespace backend {

class SimulatorCore;

} // namespace backend

class Simulator : public ISimulator {

private:

	std::unique_ptr<backend::SimulatorCore> m_core;

public:

	Simulator();
	~Simulator();

	bool RegisterSimulatorBlockFactory(design::blocks::backend::DesignBlockClass const &designBlockClass,
		std::unique_ptr<backend::ISimulatorBlockFactory> &&simulatorBlockFactory);

	void TranslateDesign(design::IDesign const &design);

	void Run(size_t cycles);

	//
	// ISimulator
	//

	virtual simulator::backend::ISimulatorAccess &GetSimulatorAccess() override;
};

} // namespace oddf::simulator::common
