#pragma once

#include <array>
#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace cli
{


template <typename T> void Parse(T &value, const char *input);


namespace details
{


inline void Parse(std::string &string, const char *input);

template <std::size_t N> void Parse(char (&array)[N], const char *input);

template <std::size_t N>
void Parse(std::array<char, N> &array, const char *input);

template <typename T> void Parse(std::optional<T> &optional, const char *input);

template <typename... Ts>
void Parse(std::vector<Ts...> &vector, const char *input);

template <typename... Ts> void Parse(std::set<Ts...> &set, const char *input);

template <typename... Ts>
void Parse(std::unordered_set<Ts...> &set, const char *input);

template <typename... Ts> void Parse(std::map<Ts...> &map, const char *input);

template <typename... Ts>
void Parse(std::unordered_map<Ts...> &map, const char *input);


} // namespace details


} // namespace cli
