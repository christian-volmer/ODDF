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

#include "../backend/IProbeAccess.h"

#include <type_traits>

namespace oddf::simulator {

template<typename integralT>
class Probe<integralT, std::enable_if_t<std::is_integral_v<integralT> && !std::is_same_v<integralT, bool>>> {

private:

	backend::IProbeAccess &m_probeAccess;
	design::NodeType const m_type;

public:

	Probe(oddf::simulator::ISimulator &simulator, std::string const &name) :
		m_probeAccess(simulator.GetSimulatorAccess().GetNamedObjectInterface<backend::IProbeAccess>(name)),
		m_type(m_probeAccess.GetType())
	{
		switch (m_type.GetTypeId()) {

			case design::NodeType::FIXED_POINT:

				if (m_type.GetFraction() > 0)
					throw Exception(ExceptionCode::Unsupported);
				break;

			default:
				throw Exception(ExceptionCode::Unsupported);
		}
	}

	integralT GetValue()
	{
		integralT rawValue { 0 };

		m_probeAccess.Read(&rawValue, sizeof(rawValue));
		if (rawValue == 0)
			return rawValue;

		if constexpr (std::is_unsigned_v<integralT>) {

			if (static_cast<size_t>(-m_type.GetFraction()) >= sizeof(integralT) * 8)
				throw Exception(ExceptionCode::Overflow);

			integralT scale = integralT { 1 } << static_cast<integralT>(-m_type.GetFraction());

			// Unsigned integer overflow is not undefined behaviour, so we can let it happen.
			integralT value = rawValue * scale;

			if (value / scale != rawValue)
				throw Exception(ExceptionCode::Overflow);

			return value;
		}
		else
			// TODO: implement for signed integer. Careful with undefined behaviour and unexpected overflows.
			throw Exception(ExceptionCode::NotImplemented);
	}
};

} // namespace oddf::simulator
