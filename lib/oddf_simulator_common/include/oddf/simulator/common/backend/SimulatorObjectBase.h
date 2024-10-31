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

    Defines `SimulatorObjectBase`, which is the base class to all simulator
    blocks.

*/

#include <oddf/Uid.h>

#pragma once

namespace oddf::simulator::common::backend {

/*
    Base class to all simulator objects.
*/
class SimulatorObjectBase {

public:

	SimulatorObjectBase(SimulatorObjectBase const &) = delete;
	void operator=(SImulatorObjectBase const &) = delete;

	SimulatorObjectBase(SimulatorObjectBase &&) = delete;
	void operator=(SImulatorObjectBase &&) = delete;

	virtual ~SimulatorObjectBase() = 0;

	void *GetInterface(Uid const &iid) = 0;
};

} // namespace oddf::simulator::common::backend
