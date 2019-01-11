#include "LightDirectional.h"

LightDirectional::LightDirectional()
{
}

LightDirectional::LightDirectional(float red, float green, float blue, Vector3D direction)
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

	_direction = direction;
}


LightDirectional::~LightDirectional()
{
}

float LightDirectional::GetRedIntensity()
{
	return _redIntesity;
}

void LightDirectional::SetRedIntensity(float value)
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

float LightDirectional::GetGreenIntensity()
{
	return _greenIntensity;
}

void LightDirectional::SetGreenIntensity(float value)
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

float LightDirectional::GetBlueIntensity()
{
	return _blueIntensity;
}

void LightDirectional::SetBlueIntensity(float value)
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

Vector3D LightDirectional::GetDirection()
{
	return _direction;
}

void LightDirectional::SetDirection(Vector3D vector)
{
	_direction = vector;
}