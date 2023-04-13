#pragma once

namespace clib_util
{
	namespace hash
	{
		// FNV-1a - https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
		template <class T>
		constexpr std::uint64_t fnv1a_64(const T& a_toHash)
		{
			constexpr std::uint64_t FNVoffsetBasis = 0xcbf29ce484222325;
			constexpr std::uint64_t FNVprime = 0x00000100000001B3;

			std::uint64_t hash = FNVoffsetBasis;

			for (const auto& ch : a_toHash) {
				hash ^= ch;
				hash *= FNVprime;
			}

			return hash;
		}

		template <class T>
		constexpr std::uint32_t fnv1a_32(const T& a_toHash)
		{
			constexpr std::uint32_t FNVoffsetBasis = 0x811c9dc5;
			constexpr std::uint64_t FNVprime = 0x01000193;

			std::uint32_t hash = FNVoffsetBasis;

			for (const auto& ch : a_toHash) {
				hash ^= ch;
				hash *= FNVprime;
			}

			return hash;
		}

		// http://szudzik.com/ElegantPairing.pdf
		constexpr std::uint64_t szudzik_pair(const std::uint32_t a_x, const std::uint32_t a_y)
		{
			return a_x >= a_y ? static_cast<std::uint64_t>((a_x * a_x) + a_x + a_y) : static_cast<std::uint64_t>((a_y * a_y) + a_x);
		}

		inline std::pair<std::uint32_t, std::uint32_t> szudzik_unpair(const std::uint64_t a_z)
		{
			const std::uint64_t sqrtz = std::floor(std::sqrt(a_z));
			const std::uint64_t sqz = sqrtz * sqrtz;

		    if ((a_z - sqz) >= sqrtz) {
				return { static_cast<std::uint32_t>(sqrtz), static_cast<std::uint32_t>(a_z - sqz - sqrtz) };
			} else {
				return { static_cast<std::uint32_t>(a_z - sqz), static_cast<std::uint32_t>(sqrtz) };
			}
		}
	}
}
