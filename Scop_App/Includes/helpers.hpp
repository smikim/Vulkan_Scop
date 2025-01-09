#pragma once

#include <algorithm>

/**
 * @brief Helper function to check size_t is correctly converted to uint32_t
 * @param value Value of type size_t to convert
 * @return An uint32_t representation of the same value
 */
template <class T>
uint32_t to_u32(T value)
{
	static_assert(std::is_arithmetic<T>::value, "T must be numeric");

	if (static_cast<uintmax_t>(value) > static_cast<uintmax_t>(std::numeric_limits<uint32_t>::max()))
	{
		throw std::runtime_error("to_u32() failed, value is too big to be converted to uint32_t");
	}

	return static_cast<uint32_t>(value);
}
