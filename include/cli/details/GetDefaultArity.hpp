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


template <typename T> constexpr Arity GetDefaultArity(const T &) noexcept
{
	if constexpr(IsArray_v<T>)
	{
		if constexpr(std::is_same_v<typename ArrayValue_t<T>, char>)
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

template <typename T>
constexpr Arity GetDefaultArity(const std::optional<T> &) noexcept
{
	const T value{};
	const Arity valueArity = GetDefaultArity(value);
	return Arity::NoMoreThan(valueArity.inclusiveMax);
}

template <typename T, typename Allocator>
constexpr Arity GetDefaultArity(const std::vector<T, Allocator> &) noexcept
{
	return Arity::Unbounded();
}

template <typename T, typename Allocator>
constexpr Arity GetDefaultArity(const std::set<T, Allocator> &)
{
	return Arity::Unbounded();
}

template <typename T, typename Hash, typename Equal, typename Allocator>
constexpr Arity
GetDefaultArity(const std::unordered_set<T, Hash, Equal, Allocator> &)
{
	return Arity::Unbounded();
}

template <typename Key, typename T, typename Compare, typename Allocator>
constexpr Arity GetDefaultArity(const std::map<Key, T, Compare, Allocator> &)
{
	return Arity::Unbounded();
}

template <
    typename Key,
    typename T,
    typename Hash,
    typename Equal,
    typename Allocator>
constexpr Arity
GetDefaultArity(const std::unordered_map<Key, T, Hash, Equal, Allocator> &)
{
	return Arity::Unbounded();
}


} // namespace details


} // namespace cli
