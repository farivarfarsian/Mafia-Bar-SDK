#pragma once
#include <utility>
#if defined  _DEBUG || defined _CRT_SECURE_NO_WARNINGS_DEBUG
#include <cassert> //Assertion
#endif //_DEBUG || _CRT_SECURE_NO_WARNINGS_DEBUG

namespace MafiaBar
{
	namespace SDK
	{
		template<class TV>
		class Vector
		{
		public:
			Vector() { ReAlloc(2); }
			template <typename... Types>
			Vector(Types&&... argv)
			{
				ReAlloc(sizeof...(Types));
				([&](auto& input) { PushBack(input); } (argv), ...);
			}
			~Vector()
			{
				Clear();
				::operator delete(mb_Data, mb_Capacity * sizeof(TV));
			}
			void Resize(size_t newCapacity)
			{
				TV* newBlock = (TV*)::operator new(newCapacity * sizeof(TV));

				if (newCapacity < mb_Size) { mb_Size = newCapacity; }

				for (size_t i = 0; i < mb_Size; i++) { newBlock[i] = std::move(mb_Data[i]); }

				for (size_t i = mb_Size; i < newCapacity; i++) { newBlock[i] = 0; }

				for (size_t i = 0; i < mb_Size; i++) { mb_Data[i].~TV(); }

				::operator delete(mb_Data, mb_Capacity * sizeof(TV)); 
				mb_Data = newBlock; 
				mb_Size = newCapacity;
				mb_Capacity = newCapacity;
			}
			void Clear()
			{
				for (size_t i = 0; i < mb_Size; i++) { mb_Data[i].~TV(); }
				mb_Size = 0;
			}
			void PushBack(const TV& datap)
			{
				if (mb_Size >= mb_Capacity) { ReAlloc(mb_Capacity + mb_Capacity / 2); }
				mb_Data[mb_Size] = datap;
				mb_Size++;
			}
			void PushBack(TV&& datap)
			{
				if (mb_Size >= mb_Capacity) { ReAlloc(mb_Capacity + mb_Capacity / 2); }
				mb_Data[mb_Size] = std::move(datap);
				mb_Size++;
			}
			void PopBack()
			{
				if (mb_Size > 0)
				{
					mb_Size--;
					mb_Data[mb_Size].~TV();
				}
			}
			void Reverse()
			{
				TV* newReverse = (TV*)::operator new(mb_Size * sizeof(TV));

				size_t IndexofReverse = 0;
				for (size_t i = mb_Size; i-- != 0;)
				{
					newReverse[IndexofReverse] = std::move(mb_Data[i]);
					IndexofReverse++;
				}

				for (size_t i = 0; i < mb_Size; i++) { mb_Data[i].~TV(); }

				::operator delete(mb_Data, mb_Capacity * sizeof(TV));
				mb_Data = newReverse;
			}
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
			constexpr TV& Front() const 
			{
				#if defined  _DEBUG || defined _CRT_SECURE_NO_WARNINGS_DEBUG
					assert(mb_Size != 0);
					return mb_Data[0];
				#else
					if (mb_Size != 0) { return mb_Data[0]; }
					else { throw "you're trying to access to an uninitialized memory"; }
				#endif //_DEBUG || _CRT_SECURE_NO_WARNINGS_DEBUG
			}
			constexpr TV& Back() const 
			{
				#if defined  _DEBUG || defined _CRT_SECURE_NO_WARNINGS_DEBUG
					assert(mb_Size != 0);
					return mb_Data[mb_Size - 1];
				#else
					if (mb_Size != 0) { return mb_Data[mb_Size - 1]; }
					else { throw "you're trying to access to an uninitialized memory"; }
				#endif //_DEBUG || _CRT_SECURE_NO_WARNINGS_DEBUG
			}
		public:
			TV& operator[](size_t index) { return mb_Data[index]; }
			constexpr TV& operator[](size_t index) const { return mb_Data[index]; }
		public:
			//Get the size of the vector
			constexpr size_t GetSize() const { return mb_Size; }
			//Get the capacity of the vector
			constexpr size_t GetCapacity() const { return mb_Capacity; }
			//Get vector Data
			constexpr TV* GetData() const { return mb_Data; }
		private:
			TV* mb_Data = nullptr;
			size_t mb_Size = 0;
			size_t mb_Capacity = 0;
			void ReAlloc(size_t newCapacity)
			{
				TV* newBlock = (TV*)::operator new(newCapacity * sizeof(TV));

				if (newCapacity < mb_Size) { mb_Size = newCapacity; }

				for (size_t i = 0; i < mb_Size; i++) { newBlock[i] = std::move(mb_Data[i]); }

				for (size_t i = 0; i < mb_Size; i++) { mb_Data[i].~TV(); }

				::operator delete(mb_Data, mb_Capacity * sizeof(TV)); //Deleting Old Data From That Old Block of Memory
				mb_Data = newBlock; //Copying Data 
				mb_Capacity = newCapacity; //Keeping The Allocaed Memory Size Block 
			}
		};
	}
}
