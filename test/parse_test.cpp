#include "cli/Parse.hpp"

#include "gtest/gtest.h"

namespace
{

TEST(parse, int)
{
	int value = -1;
	cli::Parse(value, "12");
	ASSERT_EQ(12, value);
	ASSERT_THROW(cli::Parse(value, "bad"), std::invalid_argument);
}

TEST(parse, float)
{
	float value = -1.0f;
	cli::Parse(value, "12.34");
	ASSERT_FLOAT_EQ(12.34f, value);
	ASSERT_THROW(cli::Parse(value, "bad"), std::invalid_argument);
}

TEST(parse, string)
{
	std::string string;
	std::array<char, 5> array;
	char cArray[5];

	cli::Parse(string, "test");
	ASSERT_EQ("test", string);
	cli::Parse(array, "test");
	ASSERT_STREQ("test", array.data());
	cli::Parse(cArray, "test");
	ASSERT_STREQ("test", cArray);

	ASSERT_THROW(cli::Parse(array, "testX"), std::invalid_argument);
	ASSERT_THROW(cli::Parse(cArray, "testX"), std::invalid_argument);
}

TEST(parse, optional)
{
	std::optional<int> optional;

	cli::Parse(optional, "1");
	ASSERT_TRUE(optional.has_value());
	ASSERT_EQ(1, optional.value());

	optional.reset();
	ASSERT_THROW(cli::Parse(optional, "bad"), std::invalid_argument);
	ASSERT_FALSE(optional.has_value());
}

TEST(parse, vector)
{
	std::vector<int> vector;
	cli::Parse(vector, "1");
	cli::Parse(vector, "2");

	ASSERT_EQ(2, vector.size());
	ASSERT_EQ(1, vector[0]);
	ASSERT_EQ(2, vector[1]);
}

TEST(parse, optional_vector)
{
	std::optional<std::vector<int>> maybeVector;

	ASSERT_THROW(cli::Parse(maybeVector, "bad"), std::invalid_argument);
	ASSERT_FALSE(maybeVector.has_value());

	cli::Parse(maybeVector, "1");
	cli::Parse(maybeVector, "2");
	ASSERT_TRUE(maybeVector.has_value());
	ASSERT_EQ(2, maybeVector->size());
}

TEST(parse, set)
{
	std::set<int> set;
	cli::Parse(set, "1");
	cli::Parse(set, "2");
	cli::Parse(set, "1");

	ASSERT_EQ(2, set.size());
	ASSERT_EQ(1, set.count(1));
	ASSERT_EQ(1, set.count(2));
}

TEST(parse, unordered_set)
{
	std::unordered_set<int> set;
	cli::Parse(set, "1");
	cli::Parse(set, "2");
	cli::Parse(set, "1");

	ASSERT_EQ(2, set.size());
	ASSERT_EQ(1, set.count(1));
	ASSERT_EQ(1, set.count(2));
}

TEST(parse, map)
{
	std::map<std::string, int> map;
	cli::Parse(map, "foo=1");
	cli::Parse(map, "bar=2");
	cli::Parse(map, "foo=3");

	ASSERT_EQ(2, map.size());
	ASSERT_EQ(3, map.at("foo"));
	ASSERT_EQ(2, map.at("bar"));

	ASSERT_THROW(cli::Parse(map, "cat4"), std::invalid_argument);
}

TEST(parse, unordered_map)
{
	std::unordered_map<std::string, int> map;
	cli::Parse(map, "foo=1");
	cli::Parse(map, "bar=2");
	cli::Parse(map, "foo=3");

	ASSERT_EQ(2, map.size());
	ASSERT_EQ(3, map.at("foo"));
	ASSERT_EQ(2, map.at("bar"));

	ASSERT_THROW(cli::Parse(map, "cat4"), std::invalid_argument);
}

struct Streamable
{
	bool streamCalled = false;
};

std::istream &operator>>(std::istream &stream, Streamable &streamable)
{
	streamable.streamCalled = true;
	return stream;
}

TEST(parse, streamable)
{
	Streamable value;
	cli::Parse(value, "value");
	ASSERT_TRUE(value.streamCalled);
}

struct EvilStreamable
{};

std::istream &operator>>(std::istream &stream, EvilStreamable &streamable)
{
	throw std::invalid_argument(
	    "EvilStreamable stream extraction operator called.");
}

TEST(parse, evil_streamable)
{
	EvilStreamable evil;
	ASSERT_THROW(cli::Parse(evil, "evil"), std::invalid_argument);
}

struct UserParseable : Streamable
{
	bool parseCalled = false;
};

void CLIParse(UserParseable &value, const char *input)
{
	value.parseCalled = true;
}

TEST(parse, user_parseable)
{
	UserParseable value;
	cli::Parse(value, "value");
	EXPECT_TRUE(value.parseCalled);
	EXPECT_FALSE(value.streamCalled);
}

} // namespace
