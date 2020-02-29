#pragma once

#include "cli/GenericArgument.hpp"
#include "cli/Keywords.hpp"

#include "keyword.hpp"


namespace cli
{


/// @brief Creates a command line boolean flag with an inactive value of false
/// and an active value of true.
/// @tparam Keywords Keyword argument types.
/// @param name The name of this argument.  Must have at least one leading dash.
/// @param destination The boolean destination of this flag.
/// @param keywords Keyword arguments.  Supports cli::help.
/// @returns The created argument.
template <typename... Keywords>
GenericArgument
StoreTrue(const char *name, bool &destination, Keywords... keywords)
{
	destination = false;
	keyword::Arguments kwargs{keyword::Names{help}, keywords...};
	return GenericArgument(
	    GenericArgument::Kind::BOOL,
	    name,
	    destination,
	    kwargs.GetOrDefault(help, ""));
}


/// @brief Creates a command line boolean flag with an invactive value of true
/// and an active value of false.
/// @tparam Keywords Keyword argument types.
/// @param name The name of this argument.  Must have at least one leading dash.
/// @param destination The boolean destination of this flag.
/// @param keywords Keyword arguments.  Supports cli::help.
/// @returns The created argument.
template <typename... Keywords>
GenericArgument
StoreFalse(const char *name, bool &destination, Keywords... keywords)
{
	destination = true;
	keyword::Arguments kwargs{keyword::Names{help}, keywords...};
	return GenericArgument(
	    GenericArgument::Kind::BOOL,
	    name,
	    destination,
	    kwargs.GetOrDefault(help, ""));
}


} // namespace cli
