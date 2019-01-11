#pragma once
#include "Vertex.h"
class Polygon3D
{
public:
	Polygon3D();
	Polygon3D(int index0, int index1, int index2, int uvIndex0, int uvIndex1, int uvIndex2);
	~Polygon3D();
	Polygon3D(const Polygon3D& p);

	// Accessors
	int GetIndex(int index) const;
	int GetUVIndex(int index) const;
	bool GetCull() const;
	void SetCull(const bool toCull);
	float GetDepth() const;
	void SetDepth(const float value);
	float GetRed() const;
	void SetRed(const float redValue);
	float GetGreen() const;
	void SetGreen(const int greenValue);
	float GetBlue() const;
	void SetBlue(const int blueValue);
	Vector3D GetNormal() const;
	void SetNormal(Vector3D vector);

	Polygon3D& operator= (const Polygon3D& rhs);

private:
	int _indices[3];
	int _uvIndices[3];
	bool _cull;
	float _depth;
	Vector3D _normal;
	int _red;
	int _green;
	int _blue;
};

