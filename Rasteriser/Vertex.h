#pragma once
#include "Vector3D.h"
#include "Framework.h"
class Vertex
{
public:
	//Constructors
	Vertex();
	Vertex(float x, float y, float z);
	Vertex(const Vertex& other);

	// Accessors
	float GetX() const;
	void SetX(const float x);
	float GetY() const;
	void SetY(const float y);
	float GetW() const;
	void SetW(const float w);
	float GetZ() const;
	void SetZ(const float z);
	Vector3D GetNormal() const;
	void SetNormal(const Vector3D vector);
	COLORREF GetColor() const;
	void SetColor(const COLORREF color);
	int GetTimesUsed() const;
	void SetTimesUsed(int timesUsed);
	int GetUVIndex() const;
	void SetUVIndex(int index);
	void SetU(float u);
	float GetU() const;
	void SetV(float v);
	float GetV() const;
	void SetPreTransZ(float preZ);
	float GetPreTransZ() const;
	void SetUoverZ(float UoverZ);
	float GetUoverZ() const;
	void SetVoverZ(float VoverZ);
	float GetVoverZ() const;
	void SetZRecip(float ZRecip);
	float GetZRecip() const;

	// Assignment operator
	Vertex& operator= (const Vertex& rhs);

	bool operator== (const Vertex& rhs) const;

	const Vertex operator+ (const Vertex& rhs) const;

	const Vector3D operator- (const Vertex& rhs) const;

	Vertex Dehomogenize();

private:
	float _x;
	float _y;
	float _z;
	float _w;
	Vector3D _normal;
	COLORREF _color;
	int _noOfTimesUsed;
	int _uvIndex;
	float _u;
	float _v;
	float _preTransZ;
	float _UoverZ;
	float _VoverZ;
	float _ZRecip;
};
