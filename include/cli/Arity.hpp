#pragma once

#include <cstddef>
#include <limits>


namespace cli
{

/// @brief Contains the number of arguments passed to a single command line
/// parameter.
class Arity
{
private:
	constexpr Arity(std::size_t min, std::size_t max) noexcept
	    : inclusiveMin(min)
	    , inclusiveMax(max)
	{}

public:
	static constexpr Arity Inclusive(std::size_t min, std::size_t max) noexcept
	{
		return Arity(min, max);
	}

	static constexpr Arity Exactly(std::size_t count) noexcept
	{
		return Arity(count, count);
	}

	static constexpr Arity AtLeast(std::size_t min) noexcept
	{
		return Inclusive(min, std::numeric_limits<std::size_t>::max());
	}

	static constexpr Arity GreaterThan(std::size_t exclusiveMin) noexcept
	{
		return AtLeast(exclusiveMin + 1U);
	}

	static constexpr Arity NoMoreThan(std::size_t max)
	{
		return Inclusive(0, max);
	}

	static constexpr Arity LessThan(std::size_t exclusiveMax) noexcept
	{
		return NoMoreThan(exclusiveMax - 1U);
	}

	static constexpr Arity Optional() noexcept
	{
		return Inclusive(0, 1);
	}

	static constexpr Arity Unbounded() noexcept
	{
		return AtLeast(0);
	}

	bool operator==(Arity other) const noexcept
	{
		return inclusiveMin == other.inclusiveMin
		    && inclusiveMax == other.inclusiveMax;
	}

	std::size_t inclusiveMin;
	std::size_t inclusiveMax;
};

} // namespace cli
