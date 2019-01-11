#pragma once

// Stores the U and V values for mapping textures
class TexVertex
{
public:
	TexVertex();
	TexVertex(float u, float v);
	~TexVertex();

	// Accessors
	void SetU(float u);
	float GetU();
	void SetV(float v);
	float GetV();

	// clamps a value between 0 and 1
	float ClampOne(float val);

private:
	float _u;
	float _v;
};

