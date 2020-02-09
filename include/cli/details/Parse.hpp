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
	const bool startedEmpty = !optional.has_value();
	if(startedEmpty)
	{
		optional.emplace();
	}
	try
	{
		cli::Parse(*optional, input);
	}
	catch(...)
	{
		if(startedEmpty)
		{
			optional.reset();
		}
		throw;
	}
}


template <typename... Ts>
void Parse(std::vector<Ts...> &vector, const char *input)
{
	typename std::vector<Ts...>::value_type value;
	cli::Parse(value, input);
	vector.push_back(std::move(value));
}


template <typename... Ts> void Parse(std::set<Ts...> &set, const char *input)
{
	typename std::set<Ts...>::value_type value;
	cli::Parse(value, input);
	(void)set.insert(std::move(value));
}


template <typename... Ts>
void Parse(std::unordered_set<Ts...> &set, const char *input)
{
	typename std::unordered_set<Ts...>::value_type value;
	cli::Parse(value, input);
	(void)set.insert(std::move(value));
}


template <typename... Ts> void Parse(std::map<Ts...> &map, const char *input)
{
	typename std::map<Ts...>::key_type key;
	typename std::map<Ts...>::mapped_type value;
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


template <typename... Ts>
void Parse(std::unordered_map<Ts...> &map, const char *input)
{
	typename std::unordered_map<Ts...>::key_type key;
	typename std::unordered_map<Ts...>::mapped_type value;
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
