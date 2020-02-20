#pragma once

#include "GenericArgument.hpp"

namespace cli
{


GenericArgument StoreTrue(const char *name, bool &destination, const char *help)
{
	destination = false;
	return GenericArgument(
	    GenericArgument::Kind::BOOL, name, destination, help);
}


GenericArgument
StoreFalse(const char *name, bool &destination, const char *help)
{
	destination = true;
	return GenericArgument(
	    GenericArgument::Kind::BOOL, name, destination, help);
}


} // namespace cli
