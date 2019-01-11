#pragma once

class Vector3D
{
public:
	// Constructors
	Vector3D();
	Vector3D(float x, float y, float z);

	~Vector3D();

	// Copy operator
	Vector3D& operator= (const Vector3D& rhs);
	Vector3D& operator+ (const Vector3D& rhs);

	static float DotProduct(const Vector3D vect, const Vector3D vect2);
	static Vector3D CrossProduct(const Vector3D vect, const Vector3D vect2);

	Vector3D Normalise();
	void Invert();


	// Accessors
	float GetX() const;
	void SetX(const float x);
	float GetY() const;
	void SetY(const float y);
	float GetZ() const;
	void SetZ(const float z);

private:
	float _x;
	float _y;
	float _z;
};

