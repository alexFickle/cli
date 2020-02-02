#include "gtest/gtest.h"

#include "cli/details/Destination.hpp"

namespace
{

TEST(destination, int)
{
	int value = -1;
	cli::details::Destination dest(value);
	dest.Store("1");
	ASSERT_EQ(1, value);
}

TEST(destination, int_array)
{
	// arrays with non-char values must be treated like a vector with a max size
	std::array<int, 2> array{0, 0};
	int cArray[2] = {0, 0};
	cli::details::Destination dest(array);
	cli::details::Destination cDest(cArray);

	dest.Store("1");
	cDest.Store("1");
	ASSERT_EQ(1, array[0]);
	ASSERT_EQ(1, cArray[0]);

	dest.Store("2");
	cDest.Store("2");
	ASSERT_EQ(2, array[1]);
	ASSERT_EQ(2, cArray[1]);

	ASSERT_THROW(dest.Store("3"), std::runtime_error);
	ASSERT_THROW(cDest.Store("3"), std::runtime_error);
}

TEST(destination, char_array)
{
	// char arrays must be treated like strings with a max size
	std::array<char, 5> array;
	char cArray[5];
	cli::details::Destination dest(array);
	cli::details::Destination cDest(cArray);

	ASSERT_THROW(dest.Store("testX"), std::invalid_argument);
	ASSERT_THROW(dest.Store("testX"), std::invalid_argument);

	dest.Store("test");
	cDest.Store("test");
	ASSERT_STREQ("test", array.data());
	ASSERT_STREQ("test", cArray);
}

} // namespace
