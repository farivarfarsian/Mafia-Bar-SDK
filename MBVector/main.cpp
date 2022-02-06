/*
-------------------Mafia Bar Vector-------------------
-----------------------main.cpp-----------------------
--For Testing The Vector 
*/
#include "Vector.h"
#include <iostream>
#include <vector>
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

int main()
{
	//Adding Examples Soon!
	system("pause>nul");
}