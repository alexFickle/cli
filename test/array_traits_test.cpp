#include "cli/details/ArrayTraits.hpp"

#include "gtest/gtest.h"

#include <array>
#include <type_traits>

namespace
{


TEST(array_traits, pointer)
{
	ASSERT_FALSE(cli::details::IsArray_v<char *>)
	    << "A pointer is not an array";
}

TEST(array_traits, float)
{
	ASSERT_FALSE(cli::details::IsArray_v<float>) << "A float is not an array";
}


TEST(array_traits, c_array)
{
	using array_type = int[3];
	array_type array = {-1, 0, 100};
	ASSERT_TRUE(cli::details::IsArray_v<array_type>);
	ASSERT_EQ(3, cli::details::ArraySize_v<array_type>);
	bool has_int_value_type =
	    std::is_same_v<int, typename cli::details::ArrayValue_t<array_type>>;
	ASSERT_TRUE(has_int_value_type);
	for(std::size_t i = 0; i < 3; ++i)
	{
		ASSERT_EQ(array[i], cli::details::ArrayGetData_v<array_type>(array)[i]);
	}
}

TEST(array_traits, array)
{
	using array_type = std::array<int, 3>;
	array_type array = {-1, 0, 100};
	ASSERT_TRUE(cli::details::IsArray_v<array_type>);
	ASSERT_EQ(3, cli::details::ArraySize_v<array_type>);
	bool has_int_value_type =
	    std::is_same_v<int, typename cli::details::ArrayValue_t<array_type>>;
	ASSERT_TRUE(has_int_value_type);
	for(std::size_t i = 0; i < 3; ++i)
	{
		ASSERT_EQ(array[i], cli::details::ArrayGetData_v<array_type>(array)[i]);
	}
}


} // namespace
