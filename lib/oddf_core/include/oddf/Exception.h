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

    Provides the `Exception` class and the `ExceptionCode` enum, which are the
    preferred way of reporting fatal errors in ODDF.
*/

#pragma once

#include <exception>
#include <string>
#include <cstdint>

namespace oddf {

/*
    Exception codes to be used together with the `Exception` class. Although
    they were inspired by COM error codes (HRESULTs), they have unrelated
    numerical values and are thus incompatible with COM.
*/
enum class ExceptionCode : std::uint32_t {

	// Unspecified error.
	Fail = 1,

	// Catastrophic failure.
	Unexpected,

	// The requested function has not been implemented.
	NotImplemented,

	// One or more arguments are invalid.
	InvalidArgument,

	// A method was called at an unexpected time.
	IllegalMethodCall,

	// Attempt to access data outside the valid range.
	Bounds,

	// The requested interface does not exist.
	NoInterface,

	// The requested resource does not exist.
	NoResource,

	// The requested operation is not supported.
	Unsupported,

	// The result of an operation exceeds the range of representable values of the target type.
	Overflow
};

/*
    Standard exception class for use by and with the ODDF framework. Can
    carry an exception code (of type `ExceptionCode`) and/or a message string.
*/
class Exception : public std::exception {

private:

	ExceptionCode m_code;
	std::string m_message;

public:

	// Constructs with `ExceptionCode::Fail` and custom message.
	Exception(std::string const &message) :
		m_code(ExceptionCode::Fail),
		m_message(message)
	{
	}

	// Constructs with given exception code and empty message.
	Exception(ExceptionCode code) :
		m_code(code),
		m_message()
	{
	}

	// Constructs with given exception code and message.
	Exception(ExceptionCode code, std::string const &message) :
		m_code(code),
		m_message(message)
	{
	}

	// Returns the message string.
	virtual char const *what() const noexcept override
	{
		return m_message.c_str();
	}

	// Returns the message string.
	char const *GetMessage() const noexcept
	{
		return m_message.c_str();
	}

	// Returns the exception code.
	ExceptionCode GetCode() const noexcept
	{
		return m_code;
	}
};

} // namespace oddf
