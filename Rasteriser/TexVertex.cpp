#include "TexVertex.h"



TexVertex::TexVertex()
{
}

TexVertex::TexVertex(float u, float v)
{
	_u = ClampOne(u);
	_v = ClampOne(v);
}


TexVertex::~TexVertex()
{
}

void TexVertex::SetU(float u)
{
	_u = ClampOne(u);
}

float TexVertex::GetU()
{
	return _u;
}

void TexVertex::SetV(float v)
{
	_v = ClampOne(v);
}

float TexVertex::GetV()
{
	return _v;
}

float TexVertex::ClampOne(float val)
{
	if (val > 1)
	{
		return 1;
	}
	else if (val < 0)
	{
		return 0;
	}
	else
	{
		return val;
	}
}
