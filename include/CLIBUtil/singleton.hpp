#pragma once

#include <memory>

namespace clib_util::singleton
{
	// singleton base class
	// adapted from https://github.com/Qudix/QUI-SKSE/blob/main/src/c%2B%2B/General/Singleton.hpp

    template <class T>
	class ISingleton
	{
	public:
		static T* GetSingleton()
		{
			static T singleton;
			return std::addressof(singleton);
		}

	protected:
		ISingleton() = default;
		~ISingleton() = default;

		ISingleton(const ISingleton&) = delete;
		ISingleton(ISingleton&&) = delete;
		ISingleton& operator=(const ISingleton&) = delete;
		ISingleton& operator=(ISingleton&&) = delete;
	};
}
