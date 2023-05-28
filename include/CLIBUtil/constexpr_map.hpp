#pragma once

#include <array>
#include <stdexcept>

namespace clib_util
{
	template <typename Key, typename Value, std::size_t Size>
	struct constexpr_map
	{
	    [[nodiscard]] constexpr Value at(const Key& key) const
		{
			const auto itr =
				std::find_if(begin(data), end(data),
					[&key](const auto& v) { return v.first == key; });
			if (itr != end(data)) {
				return itr->second;
			} else {
				throw std::range_error("Not Found");
			}
		}

		[[nodiscard]] constexpr Key key(const Value& value) const
		{
			const auto itr =
				std::find_if(begin(data), end(data),
					[&value](const auto& v) { return v.second == value; });
			if (itr != end(data)) {
				return itr->first;
			} else {
				throw std::range_error("Not Found");
			}
		}

		// member
		std::array<std::pair<Key, Value>, Size> data;
	};
}
