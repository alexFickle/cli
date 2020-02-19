#pragma once

#include "cli/Arity.hpp"

#include <limits>
#include <string>


namespace cli
{


namespace details
{


/// @brief Creates a usage string for a single argument.
/// @param inner The string to use inside of the arity notation to represent the
/// argument.
/// @param arity The arity of the argument.
inline std::string MakeUsageString(std::string inner, Arity arity)
{
	constexpr decltype(arity.inclusiveMax) noMax =
	    std::numeric_limits<decltype(arity.inclusiveMax)>::max();

	// specialized formatting for arities that can be expressed with using inner
	// at most twice.
	switch(arity.inclusiveMin)
	{
		case 0:
		{
			switch(arity.inclusiveMax)
			{
				case 1:
					return '[' + inner + ']';

				case 2:
					return '[' + inner + " [" + inner + "]]";

				case noMax:
					return '[' + inner + "]...";
			}
			break;
		}

		case 1:
		{
			switch(arity.inclusiveMax)
			{
				case 1:
					return inner;

				case 2:
					return inner + " [" + inner + ']';

				case noMax:
					return inner + " [" + inner + "]...";
			}
			break;
		}

		case 2:
		{
			switch(arity.inclusiveMax)
			{
				case 2:
					return inner + ' ' + inner;
			}
			break;
		}
	}

	// using regex like notation for non-specialized formatting
	if(arity.inclusiveMin == arity.inclusiveMax)
	{
		return '(' + inner + "){" + std::to_string(arity.inclusiveMin) + '}';
	}
	if(arity.inclusiveMax == noMax)
	{
		return '(' + inner + "){" + std::to_string(arity.inclusiveMin) + ",}";
	}
	return '(' + inner + "){" + std::to_string(arity.inclusiveMin) + ','
	    + std::to_string(arity.inclusiveMax) + '}';
}


} // namespace details


} // namespace cli
