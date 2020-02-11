#include "cli/CommandLine.hpp"

#include "gtest/gtest.h"

#include <array>

namespace
{


TEST(command_line, flag)
{
	std::optional<int> value;
	cli::CommandLine test(
	    "test", {cli::Argument("--flag", value, "test flag")});
	std::array<const char *, 2> args{"--flag", "1"};
	test.Run("test", 2, args.data());
	ASSERT_TRUE(value.has_value());
	ASSERT_EQ(1, *value);
}


TEST(command_line, positional)
{
	std::optional<int> value;
	cli::CommandLine test(
	    "test", {cli::Argument("value", value, "test positional")});
	std::array<const char *, 1> args{"1"};
	test.Run("test", 1, args.data());
	ASSERT_TRUE(value.has_value());
	ASSERT_EQ(1, *value);
}


} // namespace
