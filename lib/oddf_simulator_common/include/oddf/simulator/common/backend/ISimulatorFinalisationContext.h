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

#include <oddf/IObject.h>
#include "ISimulatorComponent.h"

#include <memory>

namespace oddf::simulator::common::backend {

class ISimulatorFinalisationContext {

public:

	virtual ~ISimulatorFinalisationContext() { }

	virtual void RegisterNamedObject(std::string name, std::unique_ptr<IObject> &&object) = 0;

	template<typename T, typename... argTs>
	T &CreateNamedObject(std::string name, argTs &&...args)
	{
		auto object = std::make_unique<T>(args...);
		auto &objectReference = *object;
		RegisterNamedObject(name, std::move(object));
		return objectReference;
	}

	// Returns the component that is currently being finalised.
	virtual ISimulatorComponent &GetCurrentComponent() noexcept = 0;
};

} // namespace oddf::simulator::common::backend
