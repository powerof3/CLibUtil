#pragma once

#include <filesystem>
#include <ranges>

#include "string.hpp"

namespace clib_util::config
{
	using namespace std::literals;

	inline std::vector<std::string> get_configs(std::string_view a_folder, std::string_view a_suffix, std::string_view a_extension = ".ini"sv)
	{
		std::vector<std::string> configs{};

		for (const auto iterator = std::filesystem::directory_iterator(a_folder); const auto& entry : iterator) {
			if (entry.exists()) {
				if (const auto& path = entry.path(); !path.empty() && path.extension() == a_extension) {
					if (const auto& fileName = entry.path().string(); fileName.rfind(a_suffix) != std::string::npos) {
						configs.push_back(fileName);
					}
				}
			}
		}

		std::ranges::sort(configs);

		return configs;
	}

	inline bool is_valid_entry(const std::string& a_str)
	{
		return !a_str.empty() && !string::icontains(a_str, "NONE"sv);
	}

	inline std::vector<std::string> split_entry(const std::string& a_str, std::string_view a_delimiter = ",")
	{
		return is_valid_entry(a_str) ? string::split(a_str, a_delimiter) : std::vector<std::string>();
	}
}