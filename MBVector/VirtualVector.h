#pragma once
#include <utility>

#ifndef _WIN32
	#error "VirtualVector is only supported on Windows"
#elif defined(_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#define NOCOMM
	#include <Windows.h>
	#include <yvals.h>
#endif

#define MB_VIRTUAL_VECTOR_VER 1.0f

namespace MafiaBar
{
	namespace SDK
	{
		template<class VirtualVectorClass>
		class VirtualVectorIterator final
		{
		public:
			using ValueType = typename VirtualVectorClass::Type;
			using ValuePtr = ValueType*;
			using ValueRef = ValueType&;
			using IteratorRef = VirtualVectorIterator&;
		public:
			VirtualVectorIterator(ValuePtr Ptr) : mb_Ptr(Ptr) {}
			constexpr IteratorRef operator++() noexcept
			{
				mb_Ptr++;
				return *this;
			}
			constexpr VirtualVectorIterator operator++(int) noexcept
			{
				VirtualVectorIterator _temp = *this;
				++(*this);
				return _temp;
			}
			constexpr ValueRef operator*() noexcept { return *mb_Ptr; }
			constexpr ValuePtr operator->() noexcept { return mb_Ptr; }
			constexpr bool operator==(const VirtualVectorIterator& Iterator) const noexcept { return mb_Ptr == Iterator.mb_Ptr; }
			constexpr bool operator!=(const VirtualVectorIterator& Iterator) const noexcept { return !(*this == Iterator); }
		private:
			ValuePtr mb_Ptr;
		};

		template<typename VT>
		class VirtualVector final
		{
		public:
			using Type = VT;
			using Pointer = VT*;
			using Const_Pointer = VT*;
			using Reference = VT&;
			using Const_Reference = VT&;
			using Size = size_t;
			using Iterator = VirtualVectorIterator<VirtualVector<Type>>;
			_CONSTEXPR17 VirtualVector() noexcept { ReAlloc(2); }
			_CONSTEXPR17 explicit VirtualVector(Size Alloc) { ReAlloc(Alloc); }
			_CONSTEXPR17 __stdcall VirtualVector(std::initializer_list<Type> argv)
			{
				ReAlloc(argv.size());
				for (auto element : argv) { PushBack(element); }
			}
			_CONSTEXPR17 ~VirtualVector() noexcept
			{
				Clear();
				VirtualFree(mb_Data, 0, MEM_RELEASE);
			}
			_CONSTEXPR17 void Clear() noexcept
			{
				for (Size i = 0; i < mb_Size; i++) { mb_Data[i].~VT(); }
				mb_Size = 0;
			}
			_CONSTEXPR17 void PushBack(const Type& Data)
			{
				if (mb_Size >= mb_Capacity) { ReAlloc(mb_Capacity + mb_Capacity / 2); }
				mb_Data[mb_Size] = Data;
				mb_Size++;
			}
			_CONSTEXPR17 void PushBack(Type&& Data)
			{
				if (mb_Size >= mb_Capacity) { ReAlloc(mb_Capacity + mb_Capacity / 2); }
				mb_Data[mb_Size] = std::move(Data);
				mb_Size++;
			}
			_CONSTEXPR17 void PopBack() noexcept
			{
				if (mb_Size > 0)
				{
					mb_Size--;
					mb_Data[mb_Size].~VT();
				}
			}
			_CONSTEXPR17 void Resize(Size newSize)
			{
				Pointer newBlock = static_cast<Pointer>(VirtualAlloc(NULL, newSize * sizeof(VT), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));

				if (newSize < mb_Size) { mb_Size = newSize; }

				for (Size i = 0; i < mb_Size; i++) { new(&newBlock[i]) VT(std::move(mb_Data[i])); }

				for (Size i = mb_Size; i < newSize; i++) { new(&newBlock[i]) VT(); }

				for (Size i = 0; i < mb_Size; i++) { mb_Data[i].~VT(); }

				VirtualFree(mb_Data, 0, MEM_RELEASE);
				mb_Data = newBlock;
				mb_Size = newSize;
			}
			_CONSTEXPR17 void Reverse()
			{
				Pointer newReverse = static_cast<Pointer>(VirtualAlloc(NULL, mb_Size * sizeof(Type), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));

				Size IndexofReverse = 0;
				for (Size i = mb_Size; i-- != 0;)
				{
					new(&newReverse[IndexofReverse]) VT(std::move(mb_Data[i]));
					IndexofReverse++;
				}
				for (Size i = 0; i < mb_Size; i++) { mb_Data[i].~VT(); }

				VirtualFree(mb_Data, 0, MEM_RELEASE);
				mb_Data = newReverse;
			}
			template<typename... argv>
			_CONSTEXPR17 Reference EmplaceBack(argv&&... args)
			{
				if (mb_Size >= mb_Capacity) { ReAlloc(mb_Capacity + mb_Capacity / 2); }
				new(&mb_Data[mb_Size]) VT(std::forward<argv>(args)...);
				return mb_Data[mb_Size++];
			}
			_CONSTEXPR17 void ShrinkToFit() { if (mb_Capacity > mb_Size) { ReAlloc(mb_Size); } }
			_CONSTEXPR17 bool Empty() const
			{
				if (mb_Size == 0) { return true; }
				else { return false; }
			}
			_CONSTEXPR17 Reference Front() noexcept
			{
				#if _CONTAINER_DEBUG_LEVEL > 0
					_STL_VERIFY(mb_Size != 0, "you're trying to access to an uninitialized memory (Empty Vector)");
				#else
					if (mb_Size == 0) { std::abort(); }
				#endif // _CONTAINER_DEBUG_LEVEL > 0
				return mb_Data[0];
			}
			_CONSTEXPR17 Const_Reference Front() const noexcept
			{
				#if _CONTAINER_DEBUG_LEVEL > 0
					_STL_VERIFY(mb_Size != 0, "you're trying to access to an uninitialized memory (Empty Vector)");
				#else
					if (mb_Size == 0) { std::abort(); }
				#endif // _CONTAINER_DEBUG_LEVEL > 0
				return mb_Data[0];
			}
			_CONSTEXPR17 Reference Back() noexcept
			{
				#if _CONTAINER_DEBUG_LEVEL > 0
					_STL_VERIFY(mb_Size != 0, "you're trying to access to an uninitialized memory (Empty Vector)");
				#else
					if (mb_Size == 0) { std::abort(); }
				#endif // _CONTAINER_DEBUG_LEVEL > 0
				return mb_Data[mb_Size - 1];
			}
			_CONSTEXPR17 Const_Reference Back() const noexcept
			{
				#if _CONTAINER_DEBUG_LEVEL > 0
				_STL_VERIFY(mb_Size != 0, "you're trying to access to an uninitialized memory (Empty Vector)");
				#else
				if (mb_Size == 0) { std::abort(); }
				#endif // _CONTAINER_DEBUG_LEVEL > 0
				return mb_Data[mb_Size - 1];
			}
			_CONSTEXPR17 Reference operator[](Size index) noexcept
			{
				#if _CONTAINER_DEBUG_LEVEL > 0
					_STL_VERIFY(index < mb_Size, "Wrong index: the index is out of the range of vector");
				#else
					if (index >= mb_Size) { std::abort(); }
				#endif // _CONTAINER_DEBUG_LEVEL > 0
				return mb_Data[index];
			}
			_CONSTEXPR17 Const_Reference operator[](Size index) const noexcept
			{
				#if _CONTAINER_DEBUG_LEVEL > 0
					_STL_VERIFY(index < mb_Size, "Wrong index: the index is out of the range of vector");
				#else
					if (index >= mb_Size) { std::abort(); }
				#endif // _CONTAINER_DEBUG_LEVEL > 0
				return mb_Data[index];
			}
			_CONSTEXPR17 Iterator begin() { return Iterator(&mb_Data[0]); }
			_CONSTEXPR17 Iterator end() { return Iterator(&mb_Data[mb_Size]); }
			const _CONSTEXPR17 Iterator begin() const { return Iterator(&mb_Data[0]); }
			const _CONSTEXPR17 Iterator end() const { return Iterator(&mb_Data[mb_Size]); }
			_CONSTEXPR17 Size GetSize() const noexcept { return mb_Size; }
			_CONSTEXPR17 Size GetCapacity() const noexcept { return mb_Capacity; }
			_CONSTEXPR17 Pointer GetData() noexcept { return mb_Data; }
			_CONSTEXPR17 Const_Pointer GetData() const noexcept { return mb_Data; }
		private:
			Pointer mb_Data = nullptr;
			Size mb_Size = 0;
			Size mb_Capacity = 0;
			_CONSTEXPR17 void ReAlloc(Size newCapacity)
			{
				Pointer newBlock = static_cast<Pointer>(VirtualAlloc(NULL, newCapacity * sizeof(Type), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));

				if (newCapacity < mb_Size) { mb_Size = newCapacity; }

				for (Size i = 0; i < mb_Size; i++) { new(&newBlock[i]) VT(std::move(mb_Data[i])); }

				for (Size i = 0; i < mb_Size; i++) { mb_Data[i].~VT(); }

				VirtualFree(mb_Data, 0, MEM_RELEASE);
				mb_Data = newBlock;
				mb_Capacity = newCapacity;
			}
		};
	}
}