#pragma once

#include "cli/Arity.hpp"
#include "cli/GenericArgument.hpp"
#include "cli/details/Destination.hpp"
#include "cli/details/GetDefaultArity.hpp"

namespace cli
{


/// @brief Creates a command line argument.
/// @tparam T The output type of this argument.
/// @param name The name of this argument.  If this argument is a positional
/// argument then there must be no leading dashes.  If this argument is a option
/// there must be two leading dashes.
/// @param destination The destination of this argument.
/// @param arity Contains the range of permissible times this argument may be
/// given.
/// @param help A short description of this argument that will be used in
/// generated help messages.
/// @returns The created argument.
template <typename T>
GenericArgument
Argument(const char *name, T &destination, Arity arity, const char *help)
{
	return GenericArgument(
	    name, details::Destination(destination), arity, help);
}

/// @brief Creates a command line argument with deduced arity.
/// @tparam T The output type of this argument.  The arity (range of permissible
/// times this argument maybe given) will be deduced based on this type.
/// @param name The name of this argument.  If this argument is a positional
/// argument then there must be no leading dashes.  If this argument is a option
/// there must be two leading dashes.
/// @param destination The destination of this argument.
/// @param help A short description of this argument that will be used in
/// generated help messages.
/// @returns The created argument.
template <typename T>
GenericArgument Argument(const char *name, T &destination, const char *help)
{
	return Argument(
	    name, destination, details::GetDefaultArity(destination), help);
}


} // namespace cli
