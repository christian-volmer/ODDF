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

#include "../Exception.h"
#include "../Iid.h"

namespace oddf::utility {

template<typename... interfaceTs>
struct GetInterfaceHelper;

template<>
struct GetInterfaceHelper<> {

	template<typename objectT>
	static void *GetInterface(objectT *, Uid const &)
	{
		throw Exception(ExceptionCode::NoInterface);
	}
};

template<typename firstInterfaceT, typename... restInterfaceTs>
struct GetInterfaceHelper<firstInterfaceT, restInterfaceTs...> {

	template<typename objectT>
	static void *GetInterface(objectT *object, Uid const &iid)
	{
		if (Iid<firstInterfaceT>::value == iid)
			return dynamic_cast<firstInterfaceT *>(object);
		else
			return GetInterfaceHelper<restInterfaceTs...>::GetInterface(object, iid);
	}
};

} // namespace oddf::utility
