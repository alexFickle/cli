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
Help(const char *flag, const char *help = "Prints this help message and exits.")
{
    return GenericArgument(flag, help);
}


} // namespace cli
