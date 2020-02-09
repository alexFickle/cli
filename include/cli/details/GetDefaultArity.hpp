#pragma once

#include "cli/Arity.hpp"
#include "cli/details/ArrayTraits.hpp"

#include <map>
#include <optional>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace cli
{


namespace details
{

template <typename T> constexpr Arity GetDefaultArity(const T &);

template <typename T> constexpr Arity GetDefaultArity(const std::optional<T> &);

template <typename... Ts>
constexpr Arity GetDefaultArity(const std::vector<Ts...> &);

template <typename... Ts>
constexpr Arity GetDefaultArity(const std::set<Ts...> &);

template <typename... Ts>
constexpr Arity GetDefaultArity(const std::unordered_set<Ts...> &);

template <typename... Ts>
constexpr Arity GetDefaultArity(const std::map<Ts...> &);

template <typename... Ts>
constexpr Arity GetDefaultArity(const std::unordered_map<Ts...> &);


template <typename T> constexpr Arity GetDefaultArity(const T &)
{
	if constexpr(IsArray_v<T>)
	{
		if constexpr(std::is_same_v<ArrayValue_t<T>, char>)
		{
			// string like array
			return Arity::Exactly(1);
		}
		else
		{
			return Arity::NoMoreThan(ArraySize_v<T>);
		}
	}
	else
	{
		return Arity::Exactly(1);
	}
}

template <typename T> constexpr Arity GetDefaultArity(const std::optional<T> &)
{
	const T value{};
	const Arity valueArity = GetDefaultArity(value);
	return Arity::NoMoreThan(valueArity.inclusiveMax);
}

template <typename... Ts>
constexpr Arity GetDefaultArity(const std::vector<Ts...> &)
{
	return Arity::Unbounded();
}

template <typename... Ts>
constexpr Arity GetDefaultArity(const std::set<Ts...> &)
{
	return Arity::Unbounded();
}

template <typename... Ts>
constexpr Arity GetDefaultArity(const std::unordered_set<Ts...> &)
{
	return Arity::Unbounded();
}

template <typename... Ts>
constexpr Arity GetDefaultArity(const std::map<Ts...> &)
{
	return Arity::Unbounded();
}

template <typename... Ts>
constexpr Arity GetDefaultArity(const std::unordered_map<Ts...> &)
{
	return Arity::Unbounded();
}


} // namespace details


} // namespace cli
