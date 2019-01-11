#include "Vector3D.h"
#include <math.h>

Vector3D::Vector3D()
{
}

Vector3D::Vector3D(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;
}

Vector3D::~Vector3D()
{
}

Vector3D & Vector3D::operator=(const Vector3D & rhs)
{
	if (this != &rhs)
	{
		_x = rhs.GetX();
		_y = rhs.GetY();
		_z = rhs.GetZ();
	}
	return *this;
}

Vector3D & Vector3D::operator+(const Vector3D & rhs)
{
	_x = _x + rhs.GetX();
	_y = _y + rhs.GetY();
	_z = _z + rhs.GetZ();
	return *this;
}

// Calculates the dot product of 2 vectors passed in
float Vector3D::DotProduct(const Vector3D vect, const Vector3D vect2)
{
	float result = 0;
	result = vect.GetX() * vect2.GetX() + vect.GetY() * vect2.GetY() + vect.GetZ() * vect2.GetZ();
	return result;
}


// Calculates the cross product of 2 vectors passed in
Vector3D Vector3D::CrossProduct(const Vector3D vect, const Vector3D vect2)
{
	float resultX = (vect.GetY() * vect2.GetZ()) - (vect.GetZ() * vect2.GetY());
	float resultY = (vect.GetZ()* vect2.GetX()) - (vect.GetX() * vect2.GetZ());
	float resultZ = (vect.GetX() * vect2.GetY()) - (vect.GetY() * vect2.GetX());
	Vector3D result(resultX, resultY, resultZ);
	return result;
}

// Normalise the vector
Vector3D Vector3D::Normalise()
{
	float length = (float)sqrt((_x * _x) + (_y * _y) + (_z * _z));
	float normalisedX = _x / length;
	float normalisedY = _y / length;
	float normalisedZ = _z / length;
	Vector3D normalised(normalisedX, normalisedY, normalisedZ);
	return normalised;
}

void Vector3D::Invert()
{
	_x = _x * -1;
	_y = _y * -1;
	_z = _z * -1;
}

// Accessors
float Vector3D::GetX() const
{
	return _x;
}

void Vector3D::SetX(const float x)
{
	_x = x;
}

float Vector3D::GetY() const
{
	return _y;
}

void Vector3D::SetY(const float y)
{
	_y = y;
}

float Vector3D::GetZ() const
{
	return _z;
}

void Vector3D::SetZ(const float z)
{
	_z = z;
}
