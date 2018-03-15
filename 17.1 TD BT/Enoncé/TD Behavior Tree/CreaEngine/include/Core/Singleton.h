// Singleton class as authored by Scott Bilas in the book Game Programming Gems
// Portions Copyright (C) Steve Rabin, 2001
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <assert.h>

namespace crea
{

	template <typename T>
	class Singleton
	{
	public:
		Singleton(void)
		{
			assert(ms_Singleton == nullptr);
			int offset = (int)(T*)1 - (int)(Singleton <T> *)(T*)1;
			ms_Singleton = (T*)((int)this + offset);
		}
		~Singleton(void) { assert(ms_Singleton != nullptr);  ms_Singleton = nullptr; }

		static T*   getSingleton(void) { if (ms_Singleton != nullptr) return (ms_Singleton); else return new T; }
		static bool doesSingletonExist(void) { return (ms_Singleton != nullptr); }

	private:
		static T* ms_Singleton;
	};

	template <typename T> T* Singleton <T>::ms_Singleton = nullptr;

}

#endif	// __SINGLETON_H__