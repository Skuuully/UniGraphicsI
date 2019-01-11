#include "LightPoint.h"

LightPoint::LightPoint()
{
}

LightPoint::LightPoint(float red, float green, float blue, Vertex position, float a, float b, float c)
{
	if (red > 255)
	{
		_redIntesity = 255;
	}
	else if (red < 0)
	{
		_redIntesity = 0;
	}
	else
	{
		_redIntesity = red;
	}
	if (green > 255)
	{
		_greenIntensity = 255;
	}
	else if (green < 0)
	{
		_greenIntensity = 0;
	}
	else
	{
		_greenIntensity = green;
	}
	if (blue > 255)
	{
		_blueIntensity = 255;
	}
	else if (blue < 0)
	{
		_blueIntensity = 0;
	}
	else
	{
		_blueIntensity = blue;
	}

	_position = position;
	_attenuationA = a;
	_attenuationB = b;
	_attenuationC = c;
}


LightPoint::~LightPoint()
{
}

float LightPoint::GetRedIntensity()
{
	return _redIntesity;
}

void LightPoint::SetRedIntensity(float value)
{
	if (value > 255)
	{
		_redIntesity = 255;
	}
	else if (value < 0)
	{
		_redIntesity = 0;
	}
	else
	{
		_redIntesity = value;
	}
}

float LightPoint::GetGreenIntensity()
{
	return _greenIntensity;
}

void LightPoint::SetGreenIntensity(float value)
{
	if (value > 255)
	{
		_greenIntensity = 255;
	}
	else if (value < 0)
	{
		_greenIntensity = 0;
	}
	else
	{
		_greenIntensity = value;
	}
}

float LightPoint::GetBlueIntensity()
{
	return _blueIntensity;
}

void LightPoint::SetBlueIntensity(float value)
{
	if (value > 255)
	{
		_blueIntensity = 255;
	}
	else if (value < 0)
	{
		_blueIntensity = 0;
	}
	else
	{
		_blueIntensity = value;
	}
}

Vertex LightPoint::GetPosition()
{
	return _position;
}

void LightPoint::SetPosition(Vertex position)
{
	_position = position;
}

float LightPoint::GetAttenuationA()
{
	return _attenuationA;
}

void LightPoint::SetAttenuationA(float a)
{
	_attenuationA = a;
}

float LightPoint::GetAttenuationB()
{
	return _attenuationB;
}

void LightPoint::SetAttenuationB(float b)
{
	_attenuationB = b;
}

float LightPoint::GetAttenuationC()
{
	return _attenuationC;
}

void LightPoint::SetAttenuationC(float c)
{
	_attenuationC = c;
}
