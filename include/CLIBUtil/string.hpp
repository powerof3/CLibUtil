#pragma once

#include <algorithm>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string_view>
#include <type_traits>
#include <vector>

#include "hash.hpp"

namespace clib_util
{
	namespace string
	{
		constexpr std::uint64_t const_hash(std::string_view a_str)
		{
			return hash::fnv1a_64(a_str);
		}

		inline namespace literals
		{
			constexpr std::uint64_t operator""_h(const char* a_str, std::size_t a_len)
			{
				return hash::fnv1a_64(std::string_view{ a_str, a_len });
			}
		}

		// https://stackoverflow.com/a/66897681
		inline void trim(std::string& a_str)
		{
			constexpr auto not_space = [](unsigned char ch) { return !std::isspace(ch); };

			// erase the the spaces at the back first
			// so we don't have to do extra work
			a_str.erase(
				std::ranges::find_if(a_str | std::views::reverse, not_space).base(),
				a_str.end());

			// erase the spaces at the front
			a_str.erase(
				a_str.begin(),
				std::ranges::find_if(a_str, not_space));
		}

		inline std::string trim_copy(std::string a_str)
		{
			trim(a_str);
			return a_str;
		}

		inline bool is_empty(const char* a_str)
		{
			return a_str == nullptr || a_str[0] == '\0';
		}

		inline bool is_only_digit(std::string_view a_str)
		{
			return std::ranges::all_of(a_str, [](unsigned char ch) {
				return std::isdigit(ch);
			});
		}

		inline bool is_only_hex(std::string_view a_str, bool a_requirePrefix = true)
		{
			if (!a_requirePrefix) {
				return std::ranges::all_of(a_str, [](unsigned char ch) {
					return std::isxdigit(ch);
				});
			} else if (a_str.compare(0, 2, "0x") == 0 || a_str.compare(0, 2, "0X") == 0) {
				return a_str.size() > 2 && std::all_of(a_str.begin() + 2, a_str.end(), [](unsigned char ch) {
					return std::isxdigit(ch);
				});
			}
			return false;
		}

		inline bool is_only_letter(std::string_view a_str)
		{
			return std::ranges::all_of(a_str, [](unsigned char ch) {
				return std::isalpha(ch);
			});
		}

		inline bool is_only_space(std::string_view a_str)
		{
			return std::ranges::all_of(a_str, [](unsigned char ch) {
				return std::isspace(ch);
			});
		}

		inline bool icontains(std::string_view a_str1, std::string_view a_str2)
		{
			if (a_str2.length() > a_str1.length()) {
				return false;
			}

			const auto subrange = std::ranges::search(a_str1, a_str2, [](unsigned char ch1, unsigned char ch2) {
				return std::toupper(ch1) == std::toupper(ch2);
			});

			return !subrange.empty();
		}

		inline bool iequals(std::string_view a_str1, std::string_view a_str2)
		{
			return std::ranges::equal(a_str1, a_str2, [](unsigned char ch1, unsigned char ch2) {
				return std::toupper(ch1) == std::toupper(ch2);
			});
		}

		// https://stackoverflow.com/a/35452044
		inline std::string join(const std::vector<std::string>& a_vec, std::string_view a_delimiter)
		{
			return std::accumulate(a_vec.begin(), a_vec.end(), std::string{},
				[a_delimiter](const auto& str1, const auto& str2) {
					return str1.empty() ? str2 : str1 + a_delimiter.data() + str2;
				});
		}

		template <class T>
		T to_num(const std::string& a_str, bool a_hex = false)
		{
			const int base = a_hex ? 16 : 10;

			if constexpr (std::is_same_v<T, double>) {
				return static_cast<T>(std::stod(a_str, nullptr));
			} else if constexpr (std::is_same_v<T, float>) {
				return static_cast<T>(std::stof(a_str, nullptr));
			} else if constexpr (std::is_same_v<T, std::int64_t>) {
				return static_cast<T>(std::stol(a_str, nullptr, base));
			} else if constexpr (std::is_same_v<T, std::uint64_t>) {
				return static_cast<T>(std::stoull(a_str, nullptr, base));
			} else if constexpr (std::is_signed_v<T>) {
				return static_cast<T>(std::stoi(a_str, nullptr, base));
			} else {
				return static_cast<T>(std::stoul(a_str, nullptr, base));
			}
		}

		inline std::string tolower(std::string_view a_str)
		{
			std::string result(a_str);
			std::ranges::transform(result, result.begin(), [](unsigned char ch) { return static_cast<unsigned char>(std::tolower(ch)); });
			return result;
		}

		inline std::string toupper(std::string_view a_str)
		{
			std::string result(a_str);
			std::ranges::transform(result, result.begin(), [](unsigned char ch) { return static_cast<unsigned char>(std::toupper(ch)); });
			return result;
		}

		inline std::string capitalize(std::string_view a_str, unsigned char word_delim = ' ')
		{
			std::string result(a_str);
			for (std::size_t i = 0; i < result.size(); ++i) {
				result[i] = (i == 0 || result[i - 1] == word_delim) ? std::toupper(result[i]) : std::tolower(result[i]);
			}
			return result;
		}

		inline std::string remove_non_alphanumeric(std::string& a_str)
		{
			std::ranges::replace_if(
				a_str, [](unsigned char ch) { return !std::isalnum(ch); }, ' ');
			return trim_copy(a_str);
		}

		inline std::string remove_non_numeric(std::string& a_str)
		{
			std::ranges::replace_if(
				a_str, [](unsigned char ch) { return !std::isdigit(ch); }, ' ');
			return trim_copy(a_str);
		}

		inline bool replace_all(std::string& a_str, std::string_view a_search, std::string_view a_replace)
		{
			if (a_search.empty()) {
				return false;
			}

			std::size_t pos = 0;
			bool wasReplaced = false;
			while ((pos = a_str.find(a_search, pos)) != std::string::npos) {
				a_str.replace(pos, a_search.length(), a_replace);
				pos += a_replace.length();
				wasReplaced = true;
			}

			return wasReplaced;
		}

		inline bool replace_first_instance(std::string& a_str, std::string_view a_search, std::string_view a_replace)
		{
			if (a_search.empty()) {
				return false;
			}

			if (const std::size_t pos = a_str.find(a_search); pos != std::string::npos) {
				a_str.replace(pos, a_search.length(), a_replace);
				return true;
			}

			return false;
		}

		inline bool replace_last_instance(std::string& a_str, std::string_view a_search, std::string_view a_replace)
		{
			if (a_search.empty()) {
				return false;
			}

			if (const std::size_t pos = a_str.rfind(a_search); pos != std::string::npos) {
				a_str.replace(pos, a_search.length(), a_replace);
				return true;
			}

			return false;
		}

		inline std::vector<std::string> split(const std::string& a_str, std::string_view a_delimiter)
		{
			auto range = a_str | std::ranges::views::split(a_delimiter) | std::ranges::views::transform([](auto&& r) { return std::string_view(r); });
			return { range.begin(), range.end() };
		}

		/// Parses color values provided in RGBA format.
		/// Alpha component can be ommited. Defaults to 255.
		/// 
		/// Supports the following formats:
		/// - Integer values: [0-255],[0-255],[0-255],[0-255]
		/// - Float values: [0.0-1.0],[0.0-1.0],[0.0-1.0],[0.0-1.0]
		/// - Hex values: 0xRRGGBB or 0xAARRGGBB (Note Hex values use ARGB format)
		/// - Hex values with # prefix: #RRGGBB or #AARRGGBB (Note Hex values use ARGB format)
		inline std::optional<RE::GColor> to_color(const std::string& str)
		{
			std::string trimmedStr = trim_copy(str);

			if (trimmedStr.empty()) {
				return std::nullopt;
			}

			trimmedStr = tolower(trimmedStr);

			if (trimmedStr[0] == '#') {
				trimmedStr = "0x" + trimmedStr.substr(1);  // Convert #RRGGBB to 0xRRGGBB for uniform processing
			}

			// Handle hex format: 0xRRGGBB or 0xAARRGGBB
			if (is_only_hex(trimmedStr)) {
				std::string hexDigits = trimmedStr.substr(2);  // Skip "0x"
				auto length = hexDigits.length();

				if (length != 6 && length != 8) {
					return std::nullopt;
				}

				std::uint32_t hexValue = to_num<std::uint32_t>(trimmedStr, true);

				if (length == 6) {
					hexValue += 0xFF000000;  // Add full alpha if not provided
				}

				return RE::GColor(hexValue);
			}

			// Handle comma-separated format: integer or float values
			auto components = split(trimmedStr, ",");
			if (components.size() < 3) {
				return std::nullopt;
			}

			trim(components[0]);
			trim(components[1]);
			trim(components[2]);

			if (components.size() == 4) {
				trim(components[3]);
			}

			// Check if values are floats (contain decimal point)
			bool isFloat = components[0].find('.') != std::string::npos ||
			               components[1].find('.') != std::string::npos ||
			               components[2].find('.') != std::string::npos ||
			               (components.size() == 4 && components[3].find('.') != std::string::npos);

			if (isFloat) {
				try {
					float red = to_num<float>(components[0]);
					float green = to_num<float>(components[1]);
					float blue = to_num<float>(components[2]);
					float alpha = (components.size() == 4) ? to_num<float>(components[3]) : 1.0f;

					// Clamp values to [0.0, 1.0] range
					red = std::min(1.0f, std::max(0.0f, red)) * 255.0f;
					green = std::min(1.0f, std::max(0.0f, green)) * 255.0f;
					blue = std::min(1.0f, std::max(0.0f, blue)) * 255.0f;
					alpha = std::min(1.0f, std::max(0.0f, alpha)) * 255.0f;

					return RE::GColor{
						static_cast<std::uint8_t>(red),
						static_cast<std::uint8_t>(green),
						static_cast<std::uint8_t>(blue),
						static_cast<std::uint8_t>(alpha)
					};
				} catch (...) {
					return std::nullopt;
				}
			} else {
				try {
					auto red = std::min(255, std::max(0, to_num<int>(components[0])));
					auto green = std::min(255, std::max(0, to_num<int>(components[1])));
					auto blue = std::min(255, std::max(0, to_num<int>(components[2])));
					auto alpha = (components.size() == 4) ? std::min(255, std::max(0, to_num<int>(components[3]))) : 255;

					return RE::GColor{
						static_cast<std::uint8_t>(red),
						static_cast<std::uint8_t>(green),
						static_cast<std::uint8_t>(blue),
						static_cast<std::uint8_t>(alpha)
					};
				} catch (...) {
					return std::nullopt;
				}
			}
		}

		inline RE::GColor to_color(const std::string& str, RE::GColor defaultColor)
		{
			if (auto color = to_color(str)) {
				return *color;
			}

			return defaultColor;
		}
	}
}
