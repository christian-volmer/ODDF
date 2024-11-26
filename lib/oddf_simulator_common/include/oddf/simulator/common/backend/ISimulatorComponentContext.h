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
#include <oddf/Clsid.h>
#include <oddf/Exception.h>
#include "ISimulatorComponent.h"

#include <memory>

namespace oddf::simulator::common::backend {

class ISimulatorComponentContext {

public:

	virtual ~ISimulatorComponentContext() = default;

	// Registers the given object as a global object with the simulator under the given name.
	virtual void RegisterGlobalObject(std::string name, std::unique_ptr<IObject> &&object) = 0;

	// Constructs an object of type `T` and registers it as a global object with the simulator under the given name.
	template<typename T, typename... argTs>
	T &ConstructGlobalObject(std::string name, argTs &&...args)
	{
		auto object = std::make_unique<T>(std::forward<argTs>(args)...);
		auto &objectReference = *object;
		RegisterGlobalObject(name, std::move(object));
		return objectReference;
	}

	virtual void RegisterComponentObject(Uid const &clsid, std::unique_ptr<IObject> &&object) = 0;
	virtual void *GetComponentObject(Uid const &clsid, Uid const &iid) const = 0;

	template<typename T, typename... argTs>
	T &GetOrConstructComponentObject(argTs &&...args)
	{
		IObject *pObject = nullptr;
		try {

			pObject = static_cast<IObject *>(GetComponentObject(Clsid<T>::value, Iid<IObject>::value));
		}
		catch (Exception &e) {

			if (e.GetCode() == ExceptionCode::NoResource) {

				auto upNewObject = std::make_unique<T>(std::forward<argTs>(args)...);
				auto &rNewObject = *upNewObject;
				RegisterComponentObject(Clsid<T>::value, std::move(upNewObject));
				return rNewObject;
			}

			throw;
		}

		// It is assumed that all calls to this function use the same type `T`.
		// We can therefore assume that any previously registered object is of
		// type `T` and the following cast will be valid.
		return *dynamic_cast<T *>(pObject);
	}

	// Returns the component that is currently being finalised.
	virtual ISimulatorComponent &GetCurrentComponent() noexcept = 0;
};

} // namespace oddf::simulator::common::backend
