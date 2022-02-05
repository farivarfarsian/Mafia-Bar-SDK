#pragma once
#include <utility> //Just using std::move
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
				::operator delete(mb_Data, mb_Capacity * sizeof(TV)); //Delete the Vector from Heap memory
			}
			void PushBack(const TV& datap)
			{
				//To Make Sure The Memory Size Block is Ok
				if (mb_Size >= mb_Capacity) { ReAlloc(mb_Capacity + mb_Capacity / 2); }
				mb_Data[mb_Size] = datap; //copying data
				mb_Size++;
			}
			void PushBack(TV&& datap)
			{
				//purpose of this function -> If The Arguement of PushBack Overload Function Was a rValue Then We Avoid Copying Things And use std::move to Moving The rValue
				if (mb_Size >= mb_Capacity) { ReAlloc(mb_Capacity + mb_Capacity / 2); }
				mb_Data[mb_Size] = std::move(datap); //moving data
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
			//Resize the vector
			void Resize(size_t newCapacity)
			{
				TV* newBlock = (TV*)::operator new(newCapacity * sizeof(TV));

				if (newCapacity < mb_Size) { mb_Size = newCapacity; }

				for (size_t i = 0; i < mb_Size; i++) { newBlock[i] = std::move(mb_Data[i]); } //Moving Data to a new Memory

				for (size_t i = 0; i < mb_Size; i++) { mb_Data[i].~TV(); }

				::operator delete(mb_Data, mb_Capacity * sizeof(TV)); //Deleting Old Data From That Old Block of Memory
				mb_Data = newBlock; //Copying Data 
				mb_Size = newCapacity;
				mb_Capacity = newCapacity; //Keeping The Allocaed Memory Size Block 
			}
			//Clear the whole vector
			void Clear()
			{
				for (size_t i = 0; i < mb_Size; i++) { mb_Data[i].~TV(); }
				mb_Size = 0;
			}
			template<typename... argv>
			TV& EmplaceBack(argv&&... args)
			{
				if (mb_Size >= mb_Capacity)
				{
					ReAlloc(mb_Capacity + mb_Capacity / 2);
				}
				new(&mb_Data[mb_Size]) TV(std::forward<argv>(args)...);
				return mb_Data[mb_Size++];
			}
		public: //overriding index operator for vector (both noneconst and const)
			TV& operator[](size_t index) { return mb_Data[index]; }
			const TV& operator[](size_t index) const { return mb_Data[index]; }
		public:
			//Get the size of the vector
			const size_t GetSize() const { return mb_Size; }
			//Get vector Data
			const TV* GetData() const { return mb_Data; }
		private:
			TV* mb_Data = nullptr;
			size_t mb_Size = 0;
			size_t mb_Capacity = 0;
		protected:
			void ReAlloc(size_t newCapacity)
			{
				TV* newBlock = (TV*)::operator new(newCapacity * sizeof(TV)); //Not Calling The Destructor

				if (newCapacity < mb_Size)
					mb_Size = newCapacity;

				for (size_t i = 0; i < mb_Size; i++)
					newBlock[i] = std::move(mb_Data[i]); //Avoiding Copy And Using std::move

				for (size_t i = 0; i < mb_Size; i++)
					mb_Data[i].~TV();

				::operator delete(mb_Data, mb_Capacity * sizeof(TV)); //Deleting Old Data From That Old Block of Memory
				mb_Data = newBlock; //Copying Data 
				mb_Capacity = newCapacity; //Keeping The Allocaed Memory Size Block 
			}
		};
	}
}
