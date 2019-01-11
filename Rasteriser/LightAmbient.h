#pragma once
class LightAmbient
{
public:
	LightAmbient();
	LightAmbient(float red, float green, float blue);
	~LightAmbient();

	float GetRed();
	void SetRed(float red);
	float GetGreen();
	void SetGreen(float green);
	float GetBlue();
	void SetBlue(float blue);

private:
	float _red;
	float _green;
	float _blue;
};

