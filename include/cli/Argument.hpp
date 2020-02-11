#pragma once

#include "cli/Arity.hpp"
#include "cli/details/Destination.hpp"
#include "cli/details/GetDefaultArity.hpp"

namespace cli
{


/// @brief Contains all of the information about a command line argument.
struct Argument
{
	template <typename T>
	Argument(const char *name_, T &dest_, Arity arity_, const char *help_)
	    : name(name_)
	    , dest(dest_)
	    , arity(arity_)
	    , help(help_)
	{}

	template <typename T>
	Argument(const char *name_, T &dest_, const char *help_)
	    : Argument(name_, dest_, details::GetDefaultArity(dest_), help_)
	{}

	const char *name;
	details::Destination dest;
	Arity arity;
	const char *help;
};


} // namespace cli
