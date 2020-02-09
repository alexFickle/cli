/// @file
/// @brief Contains cli::Parse()
#pragma once

#include "cli/details/ParseTraits.hpp"
#include "cli/details/Parse_fwd.hpp"

#include <sstream>
#include <type_traits>

namespace cli
{


template <typename T> void Parse(T &value, const char *input)
{
	if constexpr(details::HasUserDefinedParse_v<T>)
	{
		CLIParse(value, input);
	}
	else if constexpr(details::HasInternalParse_v<T>)
	{
		cli::details::Parse(value, input);
	}
	else if constexpr(details::HasStreamExtraction_v<T>)
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
		    "stream extraction operator or 'void CLIParse(T &value, "
		    "const char *input)'.  CLIParse() is intended to be "
		    "implemented by users externally of this library in the namespace "
		    "of the type T that is being parsed.");
	}
}


} // namespace cli


#include "cli/details/Parse.hpp"
