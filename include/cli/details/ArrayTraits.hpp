/// @file
/// @brief Contains traits for detecting and using both c-style arrays and
/// std::arrays.
#pragma once

#include <array>
#include <cstddef>

namespace cli
{


namespace details
{

/// @brief Implementation type for all traits for arrays and std::arrays.
template <typename T> struct ArrayTraits
{
	static constexpr bool is_array = false;
};

template <typename ValueType, std::size_t ArraySize>
struct ArrayTraits<std::array<ValueType, ArraySize>>
{
	static constexpr bool is_array = true;
	using value_type = ValueType;
	static constexpr std::size_t size = ArraySize;
	static ValueType *data(std::array<ValueType, ArraySize> &array)
	{
		return array.data();
	}
};

template <typename ValueType, std::size_t ArraySize>
struct ArrayTraits<ValueType[ArraySize]>
{
	static constexpr bool is_array = true;
	using value_type = ValueType;
	static constexpr std::size_t size = ArraySize;
	static ValueType *data(ValueType (&array)[ArraySize])
	{
		return +array;
	}
};

/// @brief Trait for if a type is a std::array or a c style array.
template <typename T> constexpr bool IsArray_v = ArrayTraits<T>::is_array;

/// @brief Trait for extracting the value type of an array.
template <typename T> using ArrayValue_t = typename ArrayTraits<T>::value_type;

/// @brief Trait for extracting the size of an array.
template <typename T> constexpr std::size_t ArraySize_v = ArrayTraits<T>::size;

/// @brief Trait for getting a function to get a data pointer to an array.
template <typename T> constexpr auto ArrayGetData_v = ArrayTraits<T>::data;

} // namespace details


} // namespace cli
