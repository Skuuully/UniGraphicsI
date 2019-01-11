#pragma once
#include"Vertex.h"
#include"Matrix.h"
#include <math.h>
class Camera
{
public:
	Camera();
	~Camera();

	Camera(float xRotation, float yRotation, float zRotation, const Vertex &position);

	//Accessors
	float GetxRotation();
	void SetxRotation(const float xRotation);
	float GetyRotation();
	void SetyRotation(const float yRotation);
	float GetzRotation();
	void SetzRotation(const float zRotation);
	Vertex GetPosition();
	void SetPosition(const Vertex newPos);
	void CreateMatrix();
	Matrix GetViewMatrix();


private:
	float _xRotation;
	float _yRotation;
	float _zRotation;
	Vertex _position;
	Matrix _view;

};

