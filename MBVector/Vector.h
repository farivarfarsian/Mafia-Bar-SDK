#pragma once
#include <utility> // std::move, std::initializer_list, size_t
#include <yvals.h> // Debug Macro(_CONTAINER_DEBUG_LEVEL), _STL_VERIFY

#define MB_VECTOR_VER 1.0f

namespace MafiaBar
{
	namespace SDK
	{
		template<class VectorClass>
		class VectorIterator final
		{
		public:
			using ValueType = typename VectorClass::VectorType;
			using ValuePtr = ValueType*;
			using ValueRef = ValueType&;
			using IteratorRef = VectorIterator&;
		public:
			VectorIterator(ValuePtr Ptr) : mb_Ptr(Ptr) {}
			constexpr IteratorRef operator++() noexcept
			{
				mb_Ptr++;
				return *this;
			}
			constexpr VectorIterator operator++(int) noexcept
			{
				VectorIterator _temp = *this;
				++(*this);
				return _temp;
			}
			constexpr ValueRef operator*() noexcept { return *mb_Ptr; }
			constexpr ValuePtr operator->() noexcept { return mb_Ptr; }
			constexpr bool operator==(const VectorIterator& Iterator) const noexcept { return mb_Ptr == Iterator.mb_Ptr; }
			constexpr bool operator!=(const VectorIterator& Iterator) const noexcept { return !(*this == Iterator); }
		private:
			ValuePtr mb_Ptr;
		};

		template<class TV>
		class Vector final
		{
			#ifdef _MSC_VER
				static_assert(_MSC_VER > 1910, "You're MSVC compiler is old, please use Visual Studio 2017 or upper");
				#if _MSVC_LANG < 201402L
					#error "For using Mafia Bar Vector you need at least C++14"
				#endif // _MSVC_LANG < 201402L
			#else __cplusplus < 201402L
				#error "For using Mafia Bar Vector you need at least C++14"
			#endif //_MSC_VER
			#pragma region Description about some Vector Functions
			/*
			* Why (mb_Capacity + mb_Capacity / 2) ?
			* because we want to allocate enough room for the two values that the user is gonna push in the future after those two values are pushed from the use,
			* we allocate another two blocks of memory for another two values that the user is gonna push. the algorithm of the pushing back and allocating memory goes like that.

			* Why Vector() { ReAlloc(2); }
			* because just like the algorithm explained up there, our memory allocating algorithm is like this:
			* first of all we allocate enough memory for 2 values (std::vector allocates enough memory for 4 values)
			* then we'll wait until use push back those two elements that we've allocated
			* after that we'll check if the size of the vector is equal or greater than our capacity if it was true
			* we will allocate enough memory for 2 values again and so on.

			* In the initializer_list version of the Vector constructor 
			* we don't use that two by two memory allocating algorithm anymore
			* first we just allocate enough memory to push all of those variables to memory
			* then if user push another value from that same vector, that two by two algorithm comes.
			
			* Why new(&newBlock[i]) TV(std::move(mb_Data[i])); instead of newBlock[i] = std::move(mb_Data[i]);
			* The answer is when we do " TV* newBlock = static_cast<TV*>(::operator new(newCapacity * sizeof(TV))); ", we don't actually call the constructor of the data type so in some 
			* cases it could throw an exception about "read access violation". so what we do is that we just pass our data type address and call its constructor without allocating any memory again to fix everything and then
			* move the Data to that new block and done!
			*/
			#pragma endregion
			#if 0 //retired constructor ( Vector(std::initializer_list<TV> argv) replaced it )
			template <typename... Types>
			Vector(Types&&... argv)
			{
				ReAlloc(sizeof...(Types));
				([&](auto& input) { PushBack(input); } (argv), ...);
			}
			#endif // 0
		public:
			using VectorType = TV;
			using Iterator = VectorIterator<Vector<TV>>;
			constexpr Iterator begin() { return Iterator(&mb_Data[0]); }
			constexpr Iterator end() { return Iterator(&mb_Data[mb_Size]); }
			const constexpr Iterator begin() const { return Iterator(&mb_Data[0]); }
			const constexpr Iterator end() const { return Iterator(&mb_Data[mb_Size]); }
		public:
			constexpr Vector() noexcept { ReAlloc(2); }
			constexpr explicit Vector(size_t Alloc) { ReAlloc(Alloc); }
			constexpr __stdcall Vector(std::initializer_list<TV> argv)
			{
				ReAlloc(argv.size());
				for (auto element : argv) { PushBack(element); }
			}
			constexpr ~Vector() noexcept
			{
				Clear();
				::operator delete(mb_Data, mb_Capacity * sizeof(TV));
			}
			constexpr void Resize(size_t newSize)
			{
				TV* newBlock = (TV*)::operator new(newSize * sizeof(TV));

				if (newSize < mb_Size) { mb_Size = newSize; }

				for (size_t i = 0; i < mb_Size; i++) { new(&newBlock[i]) TV(std::move(mb_Data[i])); }

				for (size_t i = mb_Size; i < newSize; i++) { new(&newBlock[i]) TV(); }

				for (size_t i = 0; i < mb_Size; i++) { mb_Data[i].~TV(); }

				::operator delete(mb_Data, mb_Capacity * sizeof(TV)); 
				mb_Data = newBlock; 
				mb_Size = newSize;
			}
			constexpr void Clear() noexcept
			{
				for (size_t i = 0; i < mb_Size; i++) { mb_Data[i].~TV(); }
				mb_Size = 0;
			}
			constexpr void PushBack(const TV& datap)
			{
				if (mb_Size >= mb_Capacity) { ReAlloc(mb_Capacity + mb_Capacity / 2); }
				mb_Data[mb_Size] = datap;
				mb_Size++;
			}
			constexpr void PushBack(TV&& datap)
			{
				if (mb_Size >= mb_Capacity) { ReAlloc(mb_Capacity + mb_Capacity / 2); }
				mb_Data[mb_Size] = std::move(datap);
				mb_Size++;
			}
			constexpr void PopBack() noexcept
			{
				if (mb_Size > 0)
				{
					mb_Size--;
					mb_Data[mb_Size].~TV();
				}
			}
			constexpr void Reverse()
			{
				TV* newReverse = (TV*)::operator new(mb_Size * sizeof(TV));

				size_t IndexofReverse = 0;
				for (size_t i = mb_Size; i-- != 0;)
				{
					new(&newReverse[IndexofReverse]) TV(std::move(mb_Data[i]));
					IndexofReverse++;
				}

				for (size_t i = 0; i < mb_Size; i++) { mb_Data[i].~TV(); }

				::operator delete(mb_Data, mb_Capacity * sizeof(TV));
				mb_Data = newReverse;
			}
			constexpr void ShrinkToFit() { if(mb_Capacity > mb_Size) { ReAlloc(mb_Size); } }
			constexpr bool Empty() const 
			{
				if (mb_Size == 0) { return true; }
				else { return false; }
			}
			template<typename... argv>
			constexpr TV& EmplaceBack(argv&&... args)
			{
				if (mb_Size >= mb_Capacity) { ReAlloc(mb_Capacity + mb_Capacity / 2); }
				new(&mb_Data[mb_Size]) TV(std::forward<argv>(args)...);
				return mb_Data[mb_Size++];
			}
			constexpr TV& Front() noexcept
			{
				#if _CONTAINER_DEBUG_LEVEL > 0
					_STL_VERIFY(mb_Size != 0, "you're trying to access to an uninitialized memory (Empty Vector)");
				#else
					if (mb_Size == 0) { std::abort(); }
				#endif // _CONTAINER_DEBUG_LEVEL > 0
				return mb_Data[0];
			}
			constexpr const TV& Front() const noexcept
			{
				#if _CONTAINER_DEBUG_LEVEL > 0
					_STL_VERIFY(mb_Size != 0, "you're trying to access to an uninitialized memory (Empty Vector)");
				#else
					if (mb_Size == 0) { std::abort(); }
				#endif // _CONTAINER_DEBUG_LEVEL > 0
				return mb_Data[0];
			}
			constexpr TV& Back() noexcept
			{
				#if _CONTAINER_DEBUG_LEVEL > 0
					_STL_VERIFY(mb_Size != 0, "you're trying to access to an uninitialized memory (Empty Vector)");
				#else
					if (mb_Size == 0) { std::abort(); }
				#endif // _CONTAINER_DEBUG_LEVEL > 0
				return mb_Data[mb_Size - 1];
			}
			constexpr const TV& Back() const noexcept
			{
				#if _CONTAINER_DEBUG_LEVEL > 0
					_STL_VERIFY(mb_Size != 0, "you're trying to access to an uninitialized memory (Empty Vector)");
				#else
					if (mb_Size == 0) { std::abort(); }
				#endif // _CONTAINER_DEBUG_LEVEL > 0
				return mb_Data[mb_Size - 1];
			}
		public:
			constexpr TV& operator[](size_t index) noexcept
			{
				#if _CONTAINER_DEBUG_LEVEL > 0
					_STL_VERIFY(index < mb_Size, "Wrong index: the index is out of the range of vector");
				#else
					if (index >= mb_Size) { std::abort(); }
				#endif // _CONTAINER_DEBUG_LEVEL > 0
				return mb_Data[index]; 
			}
			constexpr const TV& operator[](size_t index) const noexcept
			{
				#if _CONTAINER_DEBUG_LEVEL > 0
					_STL_VERIFY(index < mb_Size, "Wrong index: the index is out of the range of vector");
				#else
					if (index >= mb_Size) { std::abort(); }
				#endif // _CONTAINER_DEBUG_LEVEL > 0
				return mb_Data[index];
			}
		public:
			constexpr size_t GetSize() const noexcept { return mb_Size; }
			constexpr size_t GetCapacity() const noexcept { return mb_Capacity; }
			constexpr TV* GetData() noexcept { return mb_Data; }
			constexpr const TV* GetData() const noexcept { return mb_Data; }
		private:
			TV* mb_Data = nullptr;
			size_t mb_Size = 0;
			size_t mb_Capacity = 0;
			constexpr void ReAlloc(size_t newCapacity)
			{
				TV* newBlock = static_cast<TV*>(::operator new(newCapacity * sizeof(TV)));

				if (newCapacity < mb_Size) { mb_Size = newCapacity; }

				for (size_t i = 0; i < mb_Size; i++) { new(&newBlock[i]) TV(std::move(mb_Data[i])); }
				
				for (size_t i = 0; i < mb_Size; i++) { mb_Data[i].~TV(); }

				::operator delete(mb_Data, mb_Capacity * sizeof(TV)); 
				mb_Data = newBlock;
				mb_Capacity = newCapacity;
			}
		};
	}
}
