#pragma once
#include "Vector3D.h"
class LightDirectional
{
public:
	LightDirectional();
	LightDirectional(float red, float green, float blue, Vector3D direction);
	~LightDirectional();

	// Accessors
	float GetRedIntensity();
	void SetRedIntensity(float value);
	float GetGreenIntensity();
	void SetGreenIntensity(float value);
	float GetBlueIntensity();
	void SetBlueIntensity(float value);
	Vector3D GetDirection();
	void SetDirection(Vector3D vector);

private:
	// Intensities must be between 0 and 255 this is checked
	//for and set to max/ min depending on values passed in
	float _redIntesity;
	float _greenIntensity;
	float _blueIntensity;
	Vector3D _direction;
};

