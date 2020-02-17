/// @file
/// @brief

#pragma once

#include "cli/Parse.hpp"
#include "cli/details/ArrayTraits.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <utility>


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

public:
	Destination(const Destination &other)
	    : _dest(other._dest)
	    , _end(other._end)
	    , _storeFunction(other._storeFunction)
	{}

	Destination(Destination &other)
	    : Destination(std::as_const(other))
	{}

	template <typename T>
	Destination(T &value) noexcept
	    : _dest(nullptr)
	    , _end(nullptr)
	    , _storeFunction(nullptr)
	{
		if constexpr(IsArray_v<T>)
		{
			if constexpr(std::is_same_v<ArrayValue_t<T>, char>)
			{
				// arrays of char are treated like a bounded string, this is
				// handled by cli::Parse()
				_dest = ArrayGetData_v<T>(value);
				_storeFunction = StoreImpl<T>;
			}
			else
			{
				// arrays of non-chars are treated like a bounded vector,
				// this is handled by this class
				_dest = ArrayGetData_v<T>(value);
				_storeFunction = StoreImpl<ArrayValue_t<T>>;
				_end = ArrayGetData_v<T>(value) + ArraySize_v<T>;
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
