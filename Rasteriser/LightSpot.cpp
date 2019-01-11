#include "LightSpot.h"



LightSpot::LightSpot(float red, float green, float blue, Vertex position, float a, float b, float c, float inner, float outer)
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

	_inner = inner;
	_outer = outer;
}


LightSpot::LightSpot()
{

}

LightSpot::~LightSpot()
{
}

float LightSpot::GetRedIntensity()
{
	return _redIntesity;
}

void LightSpot::SetRedIntensity(float value)
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

float LightSpot::GetGreenIntensity()
{
	return _greenIntensity;
}

void LightSpot::SetGreenIntensity(float value)
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

float LightSpot::GetBlueIntensity()
{
	return _blueIntensity;
}

void LightSpot::SetBlueIntensity(float value)
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

Vertex LightSpot::GetPosition()
{
	return _position;
}

void LightSpot::SetPosition(Vertex position)
{
	_position = position;
}

float LightSpot::GetAttenuationA()
{
	return _attenuationA;
}

void LightSpot::SetAttenuationA(float a)
{
	_attenuationA = a;
}

float LightSpot::GetAttenuationB()
{
	return _attenuationB;
}

void LightSpot::SetAttenuationB(float b)
{
	_attenuationB = b;
}

float LightSpot::GetAttenuationC()
{
	return _attenuationC;
}

void LightSpot::SetAttenuationC(float c)
{
	_attenuationC = c;
}

float LightSpot::GetInner()
{
	return _inner;
}

void LightSpot::SetInner(float inner)
{
	_inner = inner;
}

float LightSpot::GetOuter()
{
	return _outer;
}

void LightSpot::SetOuter(float outer)
{
	_outer = outer;
}
