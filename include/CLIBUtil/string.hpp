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
	}
}
