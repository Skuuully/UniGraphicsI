#include "Vertex.h"

Vertex::Vertex()
{
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
	_w = 1.0f;
}

Vertex::Vertex(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;
	_w = 1.0f;
}

Vertex::Vertex(const Vertex & other)
{
	_x = other.GetX();
	_y = other.GetY();
	_z = other.GetZ();
	_w = other.GetW();
	_normal = other.GetNormal();
	_color = other.GetColor();
	_noOfTimesUsed = other.GetTimesUsed();
	_uvIndex = other.GetUVIndex();
	_u = other.GetU();
	_v = other.GetV();
	_preTransZ = other.GetPreTransZ();
	_UoverZ = other.GetUoverZ();
	_VoverZ = other.GetVoverZ();
	_ZRecip = other.GetZRecip();
}

// Accessors
float Vertex::GetX() const
{
	return _x;
}

void Vertex::SetX(const float x)
{
	_x = x;
}

float Vertex::GetY() const
{
	return _y;
}

void Vertex::SetY(const float y)
{
	_y = y;
}

float Vertex::GetW() const
{
	return _w;
}

void Vertex::SetW(const float w)
{
	_w = w;
}

float Vertex::GetZ() const
{
	return _z;
}

void Vertex::SetZ(const float z)
{
	_z = z;
}

Vector3D Vertex::GetNormal() const
{
	return _normal;
}

void Vertex::SetNormal(const Vector3D vector)
{
	_normal = vector;
}

COLORREF Vertex::GetColor() const
{
	return _color;
}

void Vertex::SetColor(const COLORREF color)
{
	_color = color;
}

int Vertex::GetTimesUsed() const
{
	return _noOfTimesUsed;
}

void Vertex::SetTimesUsed(int timesUsed)
{
	_noOfTimesUsed = timesUsed;
}

int Vertex::GetUVIndex() const
{
	return _uvIndex;
}

void Vertex::SetUVIndex(int index)
{
	_uvIndex = index;
}

void Vertex::SetU(float u)
{
	_u = u;
}

float Vertex::GetU() const
{
	return _u;
}

void Vertex::SetV(float v)
{
	_v = v;
}

float Vertex::GetV() const
{
	return _v;
}

void Vertex::SetPreTransZ(float preZ)
{
	_preTransZ = preZ;
}

float Vertex::GetPreTransZ() const
{
	return _preTransZ;
}

void Vertex::SetUoverZ(float UoverZ)
{
	_UoverZ = UoverZ;
}

float Vertex::GetUoverZ() const
{
	return _UoverZ;
}

void Vertex::SetVoverZ(float VoverZ)
{
	_VoverZ = VoverZ;
}

float Vertex::GetVoverZ() const
{
	return _VoverZ;
}

void Vertex::SetZRecip(float ZRecip)
{
	_ZRecip = ZRecip;
}

float Vertex::GetZRecip() const
{
	return _ZRecip;
}

Vertex& Vertex::operator=(const Vertex& rhs)
{
	// Only do the assignment if we are not assigning
	// to ourselves
	if (this != &rhs)
	{
		_x = rhs.GetX();
		_y = rhs.GetY();
		_z = rhs.GetZ();
		_w = rhs.GetW();
		_normal = rhs.GetNormal();
		_color = rhs.GetColor();
		_noOfTimesUsed = rhs.GetTimesUsed();
		_uvIndex = rhs.GetUVIndex();
		_u = rhs.GetU();
		_v = rhs.GetV();
		_preTransZ = rhs.GetPreTransZ();
		_UoverZ = rhs.GetUoverZ();
		_VoverZ = rhs.GetVoverZ();
		_ZRecip = rhs.GetZRecip();
	}
	return *this;
}

// The const at the end of the declaraion for '==" indicates that this operation does not change
// any of the member variables in this class.

bool Vertex::operator==(const Vertex& rhs) const
{
	return (_x == rhs.GetX() && _y == rhs.GetY() && _z == rhs.GetZ()/*&& rhs.GetW() */);
}

// You can see three different uses of 'const' here:
//
// The first const indicates that the method changes the return value, but it is not moved in memory
// The second const indicates that the parameter is passed by reference, but it is not modified
// The third const indicates that the operator does not change any of the memory variables in the class.

const Vertex Vertex::operator+(const Vertex& rhs) const
{
	return Vertex(_x + rhs.GetX(), _y + rhs.GetY(), _z + rhs.GetZ());
}

const Vector3D Vertex::operator-(const Vertex & rhs) const
{
	//create 3 floats that the curent vertex - the other vertex then return that as a vector
	float x = _x - rhs.GetX();
	float y = _y - rhs.GetY();
	float z = _z - rhs.GetZ();

	return Vector3D(x, y, z);
} 

//Dehomogenise the vertex by dividing the x, y, z and w components by the w component.
Vertex Vertex::Dehomogenize()
{
	// store z value befor dehomog takes place for texture perspective adjustment
	_preTransZ = _z;
	// No need to calculate the w as it is always 1 as a number divided by itself is 1
	// also when a vertex is initialised w is set to 1 anyway so no need to SetW(1)
	float x = _x / _w;
	float y = _y / _w;
	float z = _z / _w;
	_x = x;
	_y = y;
	_z = z;
	_w = 1;
	return *this;

}
