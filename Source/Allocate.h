#ifndef __ALLOCATE__
#define __ALLOCATE__

#ifdef _MSC_VER
#pragma once
#endif

#ifdef _WIN32
#include <malloc.h>
#include <Windows.h>
#elif defined(__MACH__)
#include <stdlib.h>
#include <mach/mach_init.h>
#include <mach/vm_map.h>
#elif defined(__linux__)
#include <stdlib.h>
#include <sys/mman.h>
#endif 

#ifdef _MSC_VER
#define fastcall _fastcall
#else 
#define fastcall
#endif

#ifdef _MSC_VER
#define declspec_allocator _declspec(allocator)
#else
#define declspec_allocator
#endif 

enum Allocate
{
	Heap,
	Virtual
};

namespace MafiaBar
{
	namespace SDK
	{
		template<typename T>
		declspec_allocator inline T* fastcall allocate(size_t _Size, Allocate AllocationState)
		{
			switch (AllocationState)
			{
			case Allocate::Heap:
				return static_cast<T*>(malloc(_Size));
			case Allocate::Virtual:
			#ifdef _WIN32
				return static_cast<T*>(VirtualAlloc(NULL, _Size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
			#elif defined(__MACH__) //No support yet
				void* Address = nullptr;
				vm_allocate((vm_map_t)mach_task_self(), (vm_address_t*)&Address, _Size, VM_FLAGS_ANYWHERE);
				return static_cast<T*>(Address);
			#elif defined(__linux__) //No support yet
				return  static_cast<T*>(mmap(nullptr, _Size, PROT_READ | PROT_WRITE, MAP_PRIVATE, 0, 0));
			#endif
			}
			return nullptr;
		}

		template<typename T>
		inline void deallocate(T* _Block, Allocate AllocationState, bool array = false)
		{
			switch (AllocationState)
			{
			case Allocate::Heap:
				if (array == false)
				{
					delete _Block;
				}
				else
				{
					delete[] _Block;
				}
				break;
			case Allocate::Virtual:
			#ifdef _WIN32
				VirtualFree((void*)_Block, 0, MEM_RELEASE);
				break;
			#elif defined(__MACH__) //No support yet
				break;
			#elif defined(__linux__) //No support yet
				break;
			#endif	
			}
		}
	}
}

#define alloc(Type, Size, AllocationState) MafiaBar::SDK::allocate<Type>(Size, AllocationState)

#define stack_alloc(Type, Size) static_cast<Type*>(alloca(Size))

#define dealloc(Block, AllocationState, IsArray) MafiaBar::SDK::deallocate(Block, AllocationState, IsArray) 

#endif //__ALLOCATE__