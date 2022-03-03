#pragma once
#include <utility>
#include <yvals.h>

namespace MafiaBar
{
	namespace SDK
	{
		#ifndef _MSC_VER
		#error "Stack Vector is only supported on Windows MSVC compiler"
		#endif	
		static_assert(_CONTAINER_DEBUG_LEVEL <= 0, "Stack Vector is only supported in Release mode, if you use Stack Vector in Debug mode it will endup losing data");
		static_assert(_MSVC_LANG >= 201402L, "Stack Vector supports C++14 or higher");
		template<class StackVectorClass>
		class StackVectorIterator final
		{
		public:
			using ValueType = typename StackVectorClass::StackVectorType;
			using ValuePtr = ValueType*;
			using ValueRef = ValueType&;
			using IteratorRef = StackVectorIterator&;
		public:
			StackVectorIterator(ValuePtr Ptr) : mb_Ptr(Ptr) {}
			constexpr IteratorRef operator++() noexcept
			{
				mb_Ptr++;
				return *this;
			}
			constexpr StackVectorIterator operator++(int) noexcept
			{
				StackVectorIterator _temp = *this;
				++(*this);
				return _temp;
			}
			constexpr ValueRef operator*() noexcept { return *mb_Ptr; }
			constexpr ValuePtr operator->() noexcept { return mb_Ptr; }
			constexpr bool operator==(const StackVectorIterator& Iterator) const noexcept { return mb_Ptr == Iterator.mb_Ptr; }
			constexpr bool operator!=(const StackVectorIterator& Iterator) const noexcept { return !(*this == Iterator); }
		private:
			ValuePtr mb_Ptr;
		};
		template<typename ST>
		class StackVector final
		{
		public:
			using StackVectorType = ST;
			using Iterator = StackVectorIterator<StackVector<ST>>;
			constexpr Iterator begin() { return Iterator(&mb_Data[0]); }
			constexpr Iterator end() { return Iterator(&mb_Data[mb_Size]); }
			const constexpr Iterator begin() const { return Iterator(&mb_Data[0]); }
			const constexpr Iterator end() const { return Iterator(&mb_Data[mb_Size]); }
		public:
			constexpr StackVector() noexcept { ReAlloc(2); }
			constexpr explicit StackVector(size_t Alloc) { ReAlloc(Alloc); }
			constexpr __stdcall StackVector(std::initializer_list<ST> argv)
			{
				ReAlloc(argv.size());
				for (auto element : argv) { PushBack(element); }
			}
			_CONSTEXPR20 ~StackVector() noexcept { Clear(); }
			constexpr void Resize(size_t newSize)
			{
				ST* newBlock = static_cast<ST*>(_alloca(newSize * sizeof(ST)));

				if (newSize < mb_Size) { mb_Size = newSize; }

				for (size_t i = 0; i < mb_Size; i++) { new(&newBlock[i]) ST(std::move(mb_Data[i])); }

				for (size_t i = mb_Size; i < newSize; i++) { new(&newBlock[i]) ST(); }

				for (size_t i = 0; i < mb_Size; i++) { mb_Data[i].~ST(); }

				mb_Data = newBlock;
				mb_Size = newSize;
			}
			constexpr void Clear() noexcept
			{
				for (size_t i = 0; i < mb_Size; i++) { mb_Data[i].~ST(); }
				mb_Size = 0;
			}
			constexpr void PushBack(const ST& datap)
			{
				if (mb_Size >= mb_Capacity) { ReAlloc(mb_Capacity + mb_Capacity / 2); }
				mb_Data[mb_Size] = datap;
				mb_Size++;
			}
			constexpr void PushBack(ST&& datap)
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
					mb_Data[mb_Size].~ST();
				}
			}
			constexpr void Reverse()
			{
				ST* newReverse = static_cast<ST*>(_alloca(mb_Size * sizeof(ST)));

				size_t IndexofReverse = 0;
				for (size_t i = mb_Size; i-- != 0;)
				{
					new(&newReverse[IndexofReverse]) ST(std::move(mb_Data[i]));
					IndexofReverse++;
				}

				for (size_t i = 0; i < mb_Size; i++) { mb_Data[i].~ST(); }

				mb_Data = newReverse;
			}
			constexpr void ShrinkToFit() { if (mb_Capacity > mb_Size) { ReAlloc(mb_Size); } }
			constexpr bool Empty() const
			{
				if (mb_Size == 0) { return true; }
				else { return false; }
			}
			template<typename... argv>
			constexpr ST& EmplaceBack(argv&&... args)
			{
				if (mb_Size >= mb_Capacity) { ReAlloc(mb_Capacity + mb_Capacity / 2); }
				new(&mb_Data[mb_Size]) ST(std::forward<argv>(args)...);
				return mb_Data[mb_Size++];
			}
			constexpr ST& Front() noexcept
			{
				if (mb_Size == 0) { std::abort(); }
				return mb_Data[0];
			}
			constexpr const ST& Front() const noexcept
			{
				if (mb_Size == 0) { std::abort(); }
				return mb_Data[0];
			}
			constexpr ST& Back() noexcept
			{
				if (mb_Size == 0) { std::abort(); }
				return mb_Data[mb_Size - 1];
			}
			constexpr const ST& Back() const noexcept
			{
				if (mb_Size == 0) { std::abort(); }
				return mb_Data[mb_Size - 1];
			}
		public:
			constexpr ST& operator[](size_t index) noexcept
			{
				if (index >= mb_Size) { std::abort(); }
				return mb_Data[index];
			}
			constexpr const ST& operator[](size_t index) const noexcept
			{
				if (index >= mb_Size) { std::abort(); }
				return mb_Data[index];
			}
		public:
			constexpr size_t GetSize() const noexcept { return mb_Size; }
			constexpr size_t GetCapacity() const noexcept { return mb_Capacity; }
			constexpr ST* GetData() noexcept { return mb_Data; }
			constexpr const ST* GetData() const noexcept { return mb_Data; }
		private:
			ST* mb_Data = nullptr;
			size_t mb_Size = 0;
			size_t mb_Capacity = 0;
			constexpr void ReAlloc(size_t newCapacity)
			{
				ST* newBlock = static_cast<ST*>(_alloca(sizeof(ST) * newCapacity));

				if (newCapacity < mb_Size) { mb_Size = newCapacity; }

				for (size_t i = 0; i < mb_Size; i++) { new(&newBlock[i]) ST(std::move(mb_Data[i])); }

				for (size_t i = 0; i < mb_Size; i++) { mb_Data[i].~ST(); }

				mb_Data = newBlock;
				mb_Capacity = newCapacity;
			}
		};
	}
}