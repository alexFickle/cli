#pragma once

#include "cli/Arity.hpp"

#include "keyword.hpp"


namespace cli
{


namespace details
{


struct HelpTag
{};

struct ArityTag
{};


} // namespace details


inline keyword::Name<details::HelpTag, const char *> help;

inline keyword::Name<details::ArityTag, Arity> arity;


} // namespace cli
