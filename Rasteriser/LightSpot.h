#pragma once
#include "Vertex.h"
class LightSpot
{
public:
	LightSpot();
	~LightSpot();

	LightSpot(float red, float green, float blue, Vertex position, float a, float b, float c, float inner, float outer);


	// Accessors
	float GetRedIntensity();
	void SetRedIntensity(float value);
	float GetGreenIntensity();
	void SetGreenIntensity(float value);
	float GetBlueIntensity();
	void SetBlueIntensity(float value);
	Vertex GetPosition();
	void SetPosition(Vertex position);
	float GetAttenuationA();
	void SetAttenuationA(float a);
	float GetAttenuationB();
	void SetAttenuationB(float b);
	float GetAttenuationC();
	void SetAttenuationC(float c);
	float GetInner();
	void SetInner(float inner);
	float GetOuter();
	void SetOuter(float outer);

private:
	float _redIntesity;
	float _greenIntensity;
	float _blueIntensity;
	Vertex _position;

	// A affects how well the light is lit and does not cause any changes with distamce
	// B affects how quickly the light dissipates over a distance 
	// C makes it so that the light will dissipate even quicker but then maintain a set light from after that initial distance
	float _attenuationA;
	float _attenuationB;
	float _attenuationC;

	//inner and outer angles of the cone
	float _inner;
	float _outer;

};

