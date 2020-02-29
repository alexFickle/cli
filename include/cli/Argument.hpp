#pragma once

#include "cli/Arity.hpp"
#include "cli/GenericArgument.hpp"
#include "cli/Keywords.hpp"
#include "cli/details/Destination.hpp"
#include "cli/details/GetDefaultArity.hpp"

#include "keyword.hpp"


namespace cli
{


/// @brief Creates a command line argument.
/// @tparam T The output type of this argument.
/// @tparam Keywords Keyword argument types.
/// @param name The name of this argument.  If this argument is a positional
/// argument then there must be no leading dashes.  If this argument is a option
/// there must be two leading dashes.
/// @param destination The destination of this argument.
/// @param keywords Keyword arguments.  Suports cli::help and cli::arity.
/// @returns The created argument.
template <typename T, typename... Keywords>
GenericArgument Argument(const char *name, T &destination, Keywords... keywords)
{
	keyword::Arguments kwargs{keyword::Names{help, arity}, keywords...};
	return GenericArgument(
	    GenericArgument::Kind::NORMAL,
	    name,
	    details::Destination(destination),
	    kwargs.GetOrDefault(arity, details::GetDefaultArity(destination)),
	    kwargs.GetOrDefault(help, ""));
}


} // namespace cli
