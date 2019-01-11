#include "Polygon3D.h"
#include "Vertex.h"

Polygon3D::Polygon3D()
{
}


Polygon3D::~Polygon3D()
{
}

Polygon3D::Polygon3D(int index0, int index1, int index2, int uvIndex0, int uvIndex1, int uvIndex2)
{
	_indices[0] = index0;
	_indices[1] = index1;
	_indices[2] = index2;
	_uvIndices[0] = uvIndex0;
	_uvIndices[1] = uvIndex1;
	_uvIndices[2] = uvIndex2;
	_cull = false;

	// Polygon initially black
	_red = 0;
	_green = 0;
	_blue = 0;
}

Polygon3D& Polygon3D::operator=(const Polygon3D& rhs)
{
	if (this != &rhs)
	{
		_indices[0] = rhs.GetIndex(0);
		_indices[1] = rhs.GetIndex(1);
		_indices[2] = rhs.GetIndex(2);
		_uvIndices[0] = rhs.GetUVIndex(0);
		_uvIndices[1] = rhs.GetUVIndex(1);
		_uvIndices[2] = rhs.GetUVIndex(2);
		_cull = rhs.GetCull();
		_depth = rhs.GetDepth();
		_normal = rhs.GetNormal();
		_red = (int)rhs.GetRed();
		_green = (int)rhs.GetGreen();
		_blue = (int)rhs.GetBlue();
	}
	return *this;
}

int Polygon3D :: GetIndex(int index) const
{
	return _indices[index];
}

int Polygon3D::GetUVIndex(int index) const
{
	return _uvIndices[index];
}

bool Polygon3D::GetCull() const
{
	return _cull;
}

void Polygon3D::SetCull(const bool toCull)
{
	_cull = toCull;
}

float Polygon3D::GetDepth() const
{
	return _depth;
}

void Polygon3D::SetDepth(const float value)
{
	_depth = value;
}

float Polygon3D::GetRed() const
{
	return (float)_red;
}

void Polygon3D::SetRed(const float redValue)
{
	_red = (int)redValue;
}

float Polygon3D::GetGreen() const
{
	return (float)_green;
}

void Polygon3D::SetGreen(const int greenValue)
{
	_green = greenValue;
}

float Polygon3D::GetBlue() const
{
	return (float)_blue;
}

void Polygon3D::SetBlue(const int blueValue)
{
	_blue = blueValue;
}

Vector3D Polygon3D::GetNormal() const
{
	return _normal;
}

void Polygon3D::SetNormal(Vector3D vector)
{
	_normal = vector;
}

Polygon3D::Polygon3D(const Polygon3D &poly)
{
	_indices[0] = poly.GetIndex(0);
	_indices[1] = poly.GetIndex(1);
	_indices[2] = poly.GetIndex(2);
	_uvIndices[0] = poly.GetUVIndex(0);
	_uvIndices[1] = poly.GetUVIndex(1);
	_uvIndices[2] = poly.GetUVIndex(2);
	_cull = poly.GetCull();
	_depth = poly.GetDepth();
	_normal = poly.GetNormal();
	_red = (int)poly.GetRed();
	_green = (int)poly.GetGreen();
	_blue = (int)poly.GetBlue();
}

