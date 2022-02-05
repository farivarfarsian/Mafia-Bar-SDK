/*
-------------------Mafia Bar Vector-------------------
-----------------------main.cpp-----------------------
--For Testing The Vector 
*/
#include "Vector.h"
#include <iostream>
using namespace MafiaBar::SDK;
struct Vector3
{
	float x = 0.0f, y = 0.0f, z = 0.0f;
	int* m_Meory_Block;
	Vector3() { m_Meory_Block = new int[5]; }
	Vector3(float scalar)
		: x(scalar), y(scalar), z(scalar) {
		m_Meory_Block = new int[5];
	}
	Vector3(float x, float y, float z)
		: x(x), y(y), z(z) {
		m_Meory_Block = new int[5];
	}
	Vector3(const Vector3& other) = delete;
	/* : x(other.x), y(other.y), z(other.z)
	{
		std::cout << "Copy\n" << std::endl;
	}*/
	Vector3(Vector3&& other)
		: x(other.x), y(other.y), z(other.z)
	{
		m_Meory_Block = other.m_Meory_Block;
		other.m_Meory_Block = nullptr;
		std::cout << "Move\n" << std::endl;
	}
	~Vector3()
	{
		std::cout << "Destroy\n" << std::endl;
		delete[] m_Meory_Block;
	}
	Vector3& operator=(const Vector3& other) = delete;
	/*{
		std::cout << "Copy\n";
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}*/
	Vector3& operator=(Vector3&& other)
	{
		m_Meory_Block = other.m_Meory_Block;
		other.m_Meory_Block = nullptr;
		std::cout << "Move\n";
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}
};
template<class T>
void PrintVector(const Vector<T>& Value)
{
	for (int i = 0; i < Value.GetSize(); i++)
	{
		std::cout << Value[i] << std::endl;
	}
}
void PrintVector(const Vector<Vector3>& Value)
{

	for (int i = 0; i < Value.GetSize(); i++)
	{
		std::cout << Value[i].x << ", " << Value[i].y << ", " << Value[i].z << std::endl;
	}
}
int main(void)
{
	Vector<int> why;
	why.PushBack(1);
	why.PushBack(2);
	std::cout << why.GetSize() << std::endl;
	PrintVector(why);
	why.Resize(5);
	std::cout << why.GetSize() << std::endl;
	PrintVector(why);
	auto address = why.GetData();
	getchar();
	{
		Vector<Vector3> vector;
		vector.PushBack(Vector3(1.0f));
		vector.PushBack(Vector3(2, 3 , 4));
		vector.PushBack(Vector3());
		vector.EmplaceBack(1.0f);
		vector.EmplaceBack(2, 3, 4);
		vector.EmplaceBack(1, 5, 4);
		vector.EmplaceBack();
		PrintVector(vector);
		vector.PopBack();
		vector.PopBack();
		PrintVector(vector);
		vector.EmplaceBack(5, 2, 0);
		vector.EmplaceBack(1, 7, 9);
		PrintVector(vector);
		vector.Clear();
		PrintVector(vector);
		vector.EmplaceBack(5, 2, 0);
		vector.EmplaceBack(1, 7, 9);
		PrintVector(vector);
	}
	
	Vector<int> indicies =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};
	PrintVector(indicies);
	std::cout << indicies.GetSize();
}