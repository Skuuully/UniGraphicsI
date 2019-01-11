#include "Camera.h"




Camera::Camera()
{
}


Camera::~Camera()
{
}

// Camera initialisation 
Camera::Camera(float xRotation, float yRotation, float zRotation, const Vertex &position)
{
	_xRotation = xRotation;
	_yRotation = yRotation;
	_zRotation = zRotation;
	_position = position;
	CreateMatrix();
}

// Accessors for the components of a camera
float Camera::GetxRotation()
{
	return _xRotation;
}

void Camera::SetxRotation(const float xRotation)
{
	_xRotation = xRotation;
	CreateMatrix();
}

float Camera::GetyRotation()
{
	return _yRotation;
}

void Camera::SetyRotation(const float yRotation)
{
	_yRotation = yRotation;
	CreateMatrix();
}

float Camera::GetzRotation()
{
	return _zRotation;
}

void Camera::SetzRotation(const float zRotation)
{
	_zRotation = zRotation;
	CreateMatrix();
}

Vertex Camera::GetPosition()
{
	return _position;
}

void Camera::SetPosition(const Vertex newPos)
{
	_position = newPos;
	CreateMatrix();
}

// Creates the matrix for the viewing pipeline
void Camera::CreateMatrix()
{
	_view = _view.XYZRotationMatrix(_xRotation, _yRotation, _zRotation) * _view.TranslationMatrix(-_position.GetX(), -_position.GetY(), -_position.GetZ());
}

Matrix Camera::GetViewMatrix()
{
	return _view;
}

