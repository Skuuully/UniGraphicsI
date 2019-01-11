#include "LightAmbient.h"



LightAmbient::LightAmbient()
{
}

LightAmbient::LightAmbient(float red, float green, float blue)
{
	if (red > 255)
	{
		_red = 255;
	}
	else if (red < 0)
	{
		_red = 0;
	}
	else
	{
		_red = red;
	}
	if (green > 255)
	{
		_green = 255;
	}
	else if (green < 0)
	{
		_green = 0;
	}
	else
	{
		_green = green;
	}
	if (blue > 255)
	{
		_blue = 255;
	}
	else if (blue < 0)
	{
		_blue = 0;
	}
	else
	{
		_blue = blue;
	}
}


LightAmbient::~LightAmbient()
{
}

float LightAmbient::GetRed()
{
	return _red;
}

void LightAmbient::SetRed(float red)
{
	if (red > 255)
	{
		_red = 255;
	}
	else if (red < 0)
	{
		_red = 0;
	}
	else
	{
		_red = red;
	}
}

float LightAmbient::GetGreen()
{
	return _green;
}

void LightAmbient::SetGreen(float green)
{
	if (green > 255)
	{
		_green = 255;
	}
	else if (green < 0)
	{
		_green = 0;
	}
	else
	{
		_green = green;
	}
}

float LightAmbient::GetBlue()
{
	return _blue;
}

void LightAmbient::SetBlue(float blue)
{
	if (blue > 255)
	{
		_blue = 255;
	}
	else if (blue < 0)
	{
		_blue = 0;
	}
	else
	{
		_blue = blue;
	}
}
