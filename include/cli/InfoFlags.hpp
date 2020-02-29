#pragma once

#include "cli/GenericArgument.hpp"
#include "cli/Keywords.hpp"

#include "keyword.hpp"

namespace cli
{


/// @brief Creates an argument that when used will print a help message and stop
/// command line argument parsing.
/// @tparam Keywords Keyword argument types.
/// @param flag The flag that will trigger the help message.  Must start with a
/// dash.
/// @param keywords Keyword arguments.  Supports cli::help.
/// @returns The created argument.
template <typename... Keywords>
GenericArgument Help(const char *flag, Keywords... keywords)
{
	keyword::Arguments kwargs{keyword::Names{help}, keywords...};
	return GenericArgument(
	    GenericArgument::Kind::HELP,
	    flag,
	    kwargs.GetOrDefault(help, "prints this help message and exits"));
}


/// @brief Creates an argument that when used will print a usage message and
/// stop command line argument parsing.
/// @tparam Keywords Keyword argument types.
/// @param flag The flag that will trigger the usage message.  Must start with a
/// dash.
/// @param keywords Keyword arguments.  Supports cli::help.
/// @returns The created argument.
template <typename... Keywords>
GenericArgument Usage(const char *flag, Keywords... keywords)
{
	keyword::Arguments kwargs{keyword::Names{help}, keywords...};
	return GenericArgument(
	    GenericArgument::Kind::USAGE,
	    flag,
	    kwargs.GetOrDefault(help, "prints usage and exits"));
}


/// @brief Creates an argument that when used will print a version message and
/// stop command line argument parsing.
/// @param flag The flag that will trigger the version message.  Must start with
/// a dash.
/// @param version The version string to print when requested.
/// @param keywords Keyword arguments.  Supports cli::help.
/// @returns The created argument.
template <typename... Keywords>
GenericArgument
Version(const char *flag, const char *version, Keywords... keywords)
{
	keyword::Arguments kwargs{keyword::Names{help}, keywords...};
	return GenericArgument(
	    GenericArgument::Kind::VERSION,
	    flag,
	    version,
	    kwargs.GetOrDefault(help, "prints version and exits"));
}


} // namespace cli
