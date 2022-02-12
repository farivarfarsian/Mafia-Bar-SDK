/*
-------------------Mafia Bar Vector-------------------
-----------------------main.cpp-----------------------
--For Testing The Vector 
*/
#include "Vector.h"
#include <iostream> // std::cout, std::endl, std::string

struct Vertex
{
	struct 
	{
		float x, y, z;
	} position;
};

template<class T>
void PrintVector(const MafiaBar::SDK::Vector<T>& vector) { for (int i = 0; i < vector.GetSize(); i++) { std::cout << vector[i] << std::endl; } }
void PrintVector(const MafiaBar::SDK::Vector<Vertex>& vector)
{
	for (int i = 0; i < vector.GetSize(); i++)
	{
		std::cout << "X: " << vector[i].position.x << std::endl 
				  << "Y: " << vector[i].position.y << std::endl 
			      << "Z: " << vector[i].position.z << std::endl;
	}
}

int main()
{
	std::cout << "Mafia Bar SDK Vector Version: " << MB_VECTOR_VER << std::endl;

	//Simple use of MafiaBar::SDK::Vector (First Constructor Called)
	MafiaBar::SDK::Vector<int> ints;
	ints.PushBack(1);
	ints.PushBack(2);
	ints.PushBack(3);
	std::cout << "The capacity of the Vector: " << ints.GetCapacity() << std::endl;
	std::cout << "The size of the Vector: " << ints.GetSize() << std::endl;
	std::cout << "First int: " << ints.Front() << std::endl;
	std::cout << "Last int: " << ints.Back() << std::endl;
	PrintVector(ints);

	//Sample of using EmplaceBack and ShrinkToFit Function (Second Constructor Called)
	MafiaBar::SDK::Vector<std::string> strings(10);
	strings.EmplaceBack("Hello World");
	strings.EmplaceBack("Mafia Bar");
	strings.EmplaceBack("SDK");
	std::cout << "The capacity of the Vector: " << strings.GetCapacity() << std::endl;
	std::cout << "The size of the Vector: " << strings.GetSize() << std::endl;
	strings.ShrinkToFit(); //Shrinks the capacity of vector to the size.
	std::cout << "The capacity of the Vector: " << strings.GetCapacity() << std::endl;
	std::cout << "The size of the Vector: " << strings.GetSize() << std::endl;
	PrintVector(strings);

	//Using the Third Constructor
	MafiaBar::SDK::Vector<Vertex> vertices = { {1,2,3}, {4,5,6}, {7,8,9}, {10,11,12} };
	PrintVector(vertices);


	//Sample of using Resize function
	ints.Resize(5);
	std::cout << "The capacity of the Vector: " << ints.GetCapacity() << std::endl;
	std::cout << "The size of the Vector: " << ints.GetSize() << std::endl;
	PrintVector(ints);
	strings.Resize(strings.GetSize() * 2);
	PrintVector(strings);

	//Sample of using Clear function
	ints.Clear();
	std::cout << "The capacity of the Vector: " << ints.GetCapacity() << std::endl;
	std::cout << "The size of the Vector: " << ints.GetSize() << std::endl;
	PrintVector(ints);

	//Sample of using PopBack function 
	ints.PopBack();
	std::cout << "The capacity of the Vector: " << ints.GetCapacity() << std::endl;
	std::cout << "The size of the Vector: " << ints.GetSize() << std::endl;
	PrintVector(ints);

	//Sample of using Reverse function
	ints.Reverse();
	PrintVector(ints);
	strings.Reverse();
	PrintVector(strings);

	//Sample of using Empty function
	MafiaBar::SDK::Vector<float> floats;
	if (floats.Empty() == true) { std::cout << "The vector is empty." << std::endl; }

	system("pause>nul"); //Waiting user to press any key
	return 0;
}
