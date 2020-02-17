#pragma once

#include <cstddef>
#include <stdexcept>


namespace cli
{


namespace details
{


/// @brief Yields strings from a command line invocation.
/// @details All member functions throw std::runtime_error on precondition
/// violations.
class Generator
{
public:
	/// @brief Constructor from begin and end iterators.
	Generator(const char *const *begin, const char *const *end)
	    : _next(begin)
	    , _end(end)
	{}

	/// @brief Gets the number of strings remaining in this generator.
	std::size_t Remaining() const noexcept
	{
		return static_cast<std::size_t>(_end - _next);
	}

	/// @brief Gets the next value to be yielded by this generator without
	/// consuming it.
	/// @pre Remaining() != 0.
	const char *Peek() const
	{
		if(_next == _end)
		{
			throw std::runtime_error(
			    "Internal cli error: cli::details::Generator::Peek() called on "
			    "exhausted generator.");
		}
		return *_next;
	}

	/// @brief Yields a value from this generator, consuming it.
	/// @pre Remaining() != 0.
	const char *Next()
	{
		if(_next == _end)
		{
			throw std::runtime_error(
			    "Internal cli error: cli::details::Generator::Next() called on "
			    "exhausted generator.");
		}
		const char *ret = *_next;
		_next++;
		return ret;
	}

private:
	const char *const *_next;
	const char *const *_end;
};


} // namespace details


} // namespace cli
