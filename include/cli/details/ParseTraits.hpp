#pragma once

#include "cli/details/Parse_fwd.hpp"


namespace cli
{


namespace details
{


/// @brief Detector for user defined parse functions.
/// @details A user defined parse function has the highest priority within
/// cli::Parse().  The signature of these functions are
/// "void CLIParse(T &, const char *)".
template <typename T> struct HasUserDefinedParse
{
private:
	template <typename U>
	static constexpr decltype(
	    CLIParse(std::declval<U &>(), std::declval<const char *>()),
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

template <typename T>
constexpr bool HasUserDefinedParse_v = HasUserDefinedParse<T>::value;


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

template <typename T>
constexpr bool HasInternalParse_v = HasInternalParse<T>::value;


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

template <typename T>
constexpr bool HasStreamExtraction_v = HasStreamExtraction<T>::value;


} // namespace details


} // namespace cli
