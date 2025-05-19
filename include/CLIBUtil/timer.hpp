#pragma once

#include <chrono>
#include <format>

namespace clib_util
{
	class Timer
	{
	public:
		void start()
		{
			startTime = std::chrono::steady_clock::now();
		}

		void end()
		{
			endTime = std::chrono::steady_clock::now();
		}

		[[nodiscard]] std::uint64_t duration_μs() const
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
		}

		[[nodiscard]] std::uint64_t duration_ms() const
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
		}

		[[nodiscard]] std::string duration() const
		{
			auto dur = endTime - startTime;

			if (dur < std::chrono::milliseconds(1)) {
				return std::format("{} us", std::chrono::duration_cast<std::chrono::microseconds>(dur).count());
			} else if (dur < std::chrono::seconds(1)) {
				return std::format("{} ms", std::chrono::duration_cast<std::chrono::milliseconds>(dur).count());
			} else if (dur < std::chrono::minutes(1)) {
				return std::format("{} s", std::chrono::duration_cast<std::chrono::seconds>(dur).count());
			} else [[unlikely]] {
				return "> 1 min";
			}
		}

	private:
		std::chrono::steady_clock::time_point startTime;
		std::chrono::steady_clock::time_point endTime;
	};
}
