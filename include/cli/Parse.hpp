/// @file
/// @brief Contains cli::Parse()
#pragma once

#include "cli/details/Parse.hpp"

#include <sstream>
#include <type_traits>

namespace cli
{


template <typename T> void Parse(T &value, const char *input)
{
	if constexpr(details::HasUserDefinedParse<T>::value)
	{
		cli::user::Parse(value, input);
	}
	else if constexpr(details::HasInternalParse<T>::value)
	{
		cli::details::Parse(value, input);
	}
	else if constexpr(details::HasStreamExtraction<T>::value)
	{
		std::istringstream iss(input);
		iss >> value;
		if(iss.fail())
		{
			throw std::invalid_argument("Stream extraction failed.");
		}
	}
	else
	{
		static_assert(
		    !std::is_same<T, T>::value,
		    "cli does not know how to parse this type.  Either implement a "
		    "stream extration operator or 'void cli::user::Parse(T &value, "
		    "const char *input)'.  cli::user::Parse() is intended to be "
		    "implemented by users externally of this library.");
	}
}

} // namespace cli
