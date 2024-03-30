#pragma once

#include <chrono>
#include <random>

#include "detail/XoshiroCpp.hpp"

namespace clib_util
{
	template <class T = XoshiroCpp::Xoshiro256StarStar>
	class RNG
	{
	public:
		using result_type = typename T::result_type;

		RNG() :
			_rng(std::chrono::steady_clock::now().time_since_epoch().count())
		{}
		explicit RNG(const std::uint32_t a_seed) :
			_rng(a_seed)
		{}
		explicit RNG(const std::uint64_t a_seed) :
			_rng(a_seed)
		{}

		constexpr result_type operator()() noexcept { return _rng.operator()(); };

		static constexpr result_type min() { return T::min(); }
		static constexpr result_type max() { return T::max(); }

		template <class U, class = std::enable_if_t<std::is_arithmetic_v<U>>>
		U generate(U a_min, U a_max)
		{
			if constexpr (std::is_integral_v<U>) {
				std::uniform_int_distribution<U> distr(a_min, a_max);
				return distr(_rng);
			} else {
				std::uniform_real_distribution<U> distr(a_min, a_max);
				return distr(_rng);
			}
		}

		double generate()
		{
			return XoshiroCpp::DoubleFromBits(_rng());
		}

	private:
		T _rng;
	};
}