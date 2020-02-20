#pragma once

#include "cli/GenericArgument.hpp"


namespace cli
{


/// @brief Creates an argument that when used will print a help message and stop
/// command line argument parsing.
/// @param flag The flag that will trigger the help message.  Must start with a
/// dash.
/// @param help Description of this flag.
GenericArgument
Help(const char *flag, const char *help = "prints this help message and exits")
{
	return GenericArgument(GenericArgument::Kind::HELP, flag, help);
}


/// @brief Creates an argument that when used will print a usage message and
/// stop command line argument parsing.
/// @param flag The flag that will trigger the usage message.  Must start with a
/// dash.
/// @param help Description of this flag.
GenericArgument
Usage(const char *flag, const char *help = "prints usage and exits")
{
	return GenericArgument(GenericArgument::Kind::USAGE, flag, help);
}


/// @brief Creates an argument that when used will print a version message and
/// stop command line argument parsing.
/// @param flag The flag that will trigger the version message.  Must start with
/// a dash.
/// @param version The version string to print when requested.
/// @param help Description of this flag.
GenericArgument Version(
    const char *flag,
    const char *version,
    const char *help = "prints version and exits")
{
	return GenericArgument(GenericArgument::Kind::VERSION, flag, version, help);
}


} // namespace cli
