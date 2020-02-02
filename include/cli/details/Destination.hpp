/// @file
/// @brief

#pragma once

#include "cli/Parse.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>
#include <type_traits>


namespace cli
{


namespace details
{


class Destination
{
private:
	template <typename T>
	static void StoreImpl(const char *str, void **dest, void *end)
	{
		if(*dest == end)
		{
			// arity checks should have prevented this
			throw std::runtime_error(
			    "Internal error: cli::details::Destination wrapping an array "
			    "given too many arguments.");
		}
		cli::Parse(*static_cast<T *>(*dest), str);
		if(end != nullptr)
		{
			*dest = static_cast<T *>(*dest) + 1;
		}
	}

	template <typename T> struct IsStdArray
	{
		static constexpr bool value = false;
	};

	template <typename T, std::size_t N> struct IsStdArray<std::array<T, N>>
	{
		static constexpr bool value = true;
	};

public:
	template <typename T>
	Destination(T &value) noexcept
	    : _dest(nullptr)
	    , _end(nullptr)
	    , _storeFunction(nullptr)
	{
		if constexpr(IsStdArray<T>::value)
		{
			if constexpr(std::is_same_v<typename T::value_type, char>)
			{
				// std::array<char, N> is treated like a bounded string, this is
				// handled by cli::Parse()
				_dest = &value;
				_storeFunction = StoreImpl<T>;
			}
			else
			{
				// std::array<non_char, N> is treated like a bounded vector,
				// this is handled by this class
				_dest = value.data();
				_storeFunction = StoreImpl<typename T::value_type>;
				_end = value.data() + value.size();
			}
		}
		else if constexpr(std::is_array_v<T>)
		{
			// c style arrays share the same logic as std::array
			using value_type = typename std::remove_extent_t<T>;
			if constexpr(std::is_same_v<value_type, char>)
			{
				_dest = value;
				_storeFunction = StoreImpl<T>;
			}
			else
			{
				_dest = value;
				_storeFunction = StoreImpl<value_type>;
				_end = value + (sizeof(T) / sizeof(value_type));
			}
		}
		else
		{
			// non-arrays are handled by cli::Parse()
			_dest = &value;
			_storeFunction = StoreImpl<T>;
		}
	}

	void Store(const char *str)
	{
		_storeFunction(str, &_dest, _end);
	}

private:
	void *_dest;
	void *_end;
	void (*_storeFunction)(const char *, void **, void *);
};


} // namespace details


} // namespace cli
