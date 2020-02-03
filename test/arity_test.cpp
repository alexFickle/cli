#include "cli/Arity.hpp"
#include "cli/details/GetDefaultArity.hpp"

#include "gtest/gtest.h"

namespace
{

/////////////////////////
// arity one

template <typename T> struct one : ::testing::Test
{};

TYPED_TEST_CASE_P(one);

TYPED_TEST_P(one, normal)
{
	TypeParam value;
	ASSERT_EQ(cli::Arity::Exactly(1), cli::details::GetDefaultArity(value));
}

TYPED_TEST_P(one, optional)
{
	std::optional<TypeParam> value;
	ASSERT_EQ(cli::Arity::Optional(), cli::details::GetDefaultArity(value));
}

REGISTER_TYPED_TEST_CASE_P(one, normal, optional);

using OneTypes =
    ::testing::Types<char, int, float, std::string, std::array<char, 30>>;

INSTANTIATE_TYPED_TEST_CASE_P(arity, one, OneTypes);

// arity one
/////////////////////////
// arity unbounded

template <typename T> struct unbounded : ::testing::Test
{};

TYPED_TEST_CASE_P(unbounded);

TYPED_TEST_P(unbounded, normal)
{
	TypeParam value;
	ASSERT_EQ(cli::Arity::Unbounded(), cli::details::GetDefaultArity(value));
}

TYPED_TEST_P(unbounded, optional)
{
	std::optional<TypeParam> value;
	ASSERT_EQ(cli::Arity::Unbounded(), cli::details::GetDefaultArity(value));
}

REGISTER_TYPED_TEST_CASE_P(unbounded, normal, optional);

using UnboundedTypes = ::testing::Types<
    std::vector<int>,
    std::set<int>,
    std::unordered_set<int>,
    std::map<int, std::string>,
    std::unordered_map<int, std::string>>;

INSTANTIATE_TYPED_TEST_CASE_P(arity, unbounded, UnboundedTypes);

// arity unbounded
/////////////////////////
// arrays

TEST(arity, char_array)
{
	std::array<char, 5> array;
	char cArray[5];

	ASSERT_EQ(cli::Arity::Exactly(1), cli::details::GetDefaultArity(array));
	ASSERT_EQ(cli::Arity::Exactly(1), cli::details::GetDefaultArity(cArray));
}

TEST(arity, int_array)
{
	std::array<int, 5> array;
	int cArray[5];

	ASSERT_EQ(cli::Arity::NoMoreThan(5), cli::details::GetDefaultArity(array));
	ASSERT_EQ(cli::Arity::NoMoreThan(5), cli::details::GetDefaultArity(cArray));
}

} // namespace
