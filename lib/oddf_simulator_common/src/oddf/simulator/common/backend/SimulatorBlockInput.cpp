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

#include <oddf/simulator/common/backend/SimulatorBlockBase.h>
#include <oddf/Exception.h>

#include <algorithm>
#include <stdexcept>

namespace oddf::simulator::common::backend {

SimulatorBlockInput::SimulatorBlockInput(SimulatorBlockBase &owningBlock, size_t index) :
	m_owningBlock(owningBlock),
	m_driver(nullptr),
	m_index(index),
	m_inputPointerReference(0)
{
}

SimulatorBlockInput::SimulatorBlockInput(SimulatorBlockInput &&other) :
	m_owningBlock(other.m_owningBlock),
	m_driver(nullptr),
	m_index(0),
	m_inputPointerReference(0)
{
	// Since the address of an input object must never change, it cannot be moved.
	throw oddf::Exception(oddf::ExceptionCode::Unexpected);
}

design::NodeType SimulatorBlockInput::GetType() const
{
	if (IsConnected())
		return GetDriver().GetType();
	else
		throw Exception(ExceptionCode::IllegalMethodCall, "SimulatorBlockInput::GetType(): The input has no driver.");
}

size_t SimulatorBlockInput::GetIndex() const
{
	return m_index;
}

SimulatorBlockBase const &SimulatorBlockInput::GetOwningBlock() const
{
	return m_owningBlock;
}

SimulatorBlockBase &SimulatorBlockInput::GetOwningBlock()
{
	return m_owningBlock;
}

bool SimulatorBlockInput::IsConnected() const
{
	return m_driver != nullptr;
}

SimulatorBlockOutput const &SimulatorBlockInput::GetDriver() const
{
	if (!IsConnected())
		throw Exception(ExceptionCode::IllegalMethodCall);

	return *m_driver;
}

SimulatorBlockOutput &SimulatorBlockInput::GetDriver()
{
	if (!IsConnected())
		throw Exception(ExceptionCode::IllegalMethodCall);

	return *m_driver;
}

void SimulatorBlockInput::ConnectTo(SimulatorBlockOutput &output)
{
	if (IsConnected())
		throw std::logic_error("SimulatorBlockInput::ConnectTo(): this input is already connected.");

	m_driver = &output;
	output.m_targets.push_back(this);
}

void SimulatorBlockInput::Disconnect()
{
	if (!IsConnected())
		throw std::logic_error("SimulatorBlockInput::Disconnect(): this input is unconnected.");

	auto it = std::find(m_driver->m_targets.cbegin(), m_driver->m_targets.cend(), this);

	if (it != m_driver->m_targets.cend())
		m_driver->m_targets.erase(it);
	else
		throw std::runtime_error("SimulatorBlockInput::Disconnect(): internal error. The input-output connection is inconsistent.");

	m_driver = nullptr;
}

} // namespace oddf::simulator::common::backend
