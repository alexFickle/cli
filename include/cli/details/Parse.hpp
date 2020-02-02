/// @file
/// @brief Contains implementation details for cli::Parse() including parsers
/// for standard library types.
#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstring>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>


namespace cli
{


namespace details
{


/// @brief Detector for user defined parse functions.
/// @details A user defined parse function has the highest priority within
/// cli::Parse().  The signature of these functions are
/// "void cli::user::Parse(T &, const char *)".
template <typename T> struct HasUserDefinedParse
{
private:
	template <typename U>
	static constexpr decltype(
	    cli::user::Parse(std::declval<U &>(), std::declval<const char *>()),
	    bool())
	Test(int)
	{
		return true;
	}

	template <typename U> static constexpr bool Test(...)
	{
		return false;
	}

public:
	static constexpr bool value = Test<T>(int());
};


/// @brief Detector for an internal parse function.
/// @details These parse functions have the signature
/// "void cli::details::Parse(T &, const char *)" and are considered if
/// no suitable user defined parser exists.
template <typename T> struct HasInternalParse
{
private:
	template <typename U>
	static constexpr decltype(
	    cli::details::Parse(std::declval<U &>(), std::declval<const char *>()),
	    bool())
	Test(int)
	{
		return true;
	}

	template <typename U> static constexpr bool Test(...)
	{
		return false;
	}

public:
	static constexpr bool value = Test<T>(int());
};


/// @brief Detector for a stream extraction operator.
/// @details Using a stream extraction operator with a std::istringstream is
/// considered only if there is no suitable user defined or internal parser.
template <typename T> struct HasStreamExtraction
{
private:
	template <typename U>
	static constexpr decltype(
	    std::declval<std::istringstream>() >> std::declval<U &>(),
	    bool())
	Test(int)
	{
		return true;
	}

	template <typename U> static constexpr bool Test(...)
	{
		return false;
	}

public:
	static constexpr bool value = Test<T>(int());
};


inline void Parse(std::string &string, const char *input)
{
	string = input;
}


template <std::size_t N> void Parse(char (&array)[N], const char *input)
{
	std::size_t length = std::strlen(input);
	if(length >= N)
	{
		throw std::invalid_argument(
		    "Command line argument does not fit in fixed size character array");
	}
	std::memcpy(array, input, length + 1);
}


template <std::size_t N>
void Parse(std::array<char, N> &array, const char *input)
{
	std::size_t length = std::strlen(input);
	if(length >= N)
	{
		throw std::invalid_argument(
		    "Command line argument does not fit in fixed size character array");
	}
	std::memcpy(&array[0], input, length + 1);
}


template <typename T> void Parse(std::optional<T> &optional, const char *input)
{
	optional.emplace();
	try
	{
		cli::Parse(*optional, input);
	}
	catch(...)
	{
		optional.reset();
		throw;
	}
}


template <typename T, typename Allocator>
void Parse(std::vector<T, Allocator> &vector, const char *input)
{
	T value;
	cli::Parse(value, input);
	vector.push_back(std::move(value));
}


template <typename T, typename Allocator>
void Parse(std::set<T, Allocator> &set, const char *input)
{
	T value;
	cli::Parse(value, input);
	(void)set.insert(std::move(value));
}


template <typename T, typename Hash, typename Equal, typename Allocator>
void Parse(
    std::unordered_set<T, Hash, Equal, Allocator> &set,
    const char *input)
{
	T value;
	cli::Parse(value, input);
	(void)set.insert(std::move(value));
}


template <typename Key, typename T, typename Compare, typename Allocator>
void Parse(std::map<Key, T, Compare, Allocator> &map, const char *input)
{
	Key key;
	T value;
	const char *const end = input + std::strlen(input);
	const char *const equal = std::find(input, end, '=');
	if(equal == end)
	{
		throw std::invalid_argument(
		    "Command line argument not in <key>=<value> form");
	}
	cli::Parse(key, std::string(input, equal - input).c_str());
	cli::Parse(value, equal + 1);
	(void)map.insert_or_assign(std::move(key), std::move(value));
}


template <
    typename Key,
    typename T,
    typename Hash,
    typename Equal,
    typename Allocator>
void Parse(
    std::unordered_map<Key, T, Hash, Equal, Allocator> &map,
    const char *input)
{
	Key key;
	T value;
	const char *const end = input + std::strlen(input);
	const char *const equal = std::find(input, end, '=');
	if(equal == end)
	{
		throw std::invalid_argument(
		    "Command line argument not in <key>=<value> form");
	}
	cli::Parse(key, std::string(input, equal - input).c_str());
	cli::Parse(value, equal + 1);
	(void)map.insert_or_assign(std::move(key), std::move(value));
}


} // namespace details


} // namespace cli
