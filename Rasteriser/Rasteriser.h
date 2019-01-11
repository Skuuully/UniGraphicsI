#pragma once
#include <vector>
#include "Framework.h"
#include "Vertex.h"
#include "Matrix.h"
#include "Model.h"
#include "Camera.h"
#include "MD2Loader.h"
#include "Vector3D.h"
#include "LightDirectional.h"
#include "LightAmbient.h"
#include "LightPoint.h"
#include <algorithm>
#include <math.h>
#include <ctime>

class Rasteriser : public Framework
{
public:
	bool Initialise();
	void Update(Bitmap &bitmap);
	void Render(Bitmap &bitmap);

	// Generate matrices used in the graphics pipeline
	void GeneratePerspectiveMatrix(float d, float aspectRatio);
	void GenerateScreenMatrix(float d, int width, int height);

	// Draws the base wireframe
	void DrawWireFrame(Bitmap &bitmap);
	void DrawSolidFlat(Bitmap &bitmap);
	void MyDrawSolidFlat(Bitmap & bitmap);
	void DrawGouraud(Bitmap &bitmap);
	void DrawSolidTextured(Bitmap &bitmap);
	void DrawSolidTexturedCorrected(Bitmap &bitmap);

	void DrawLine(Bitmap &bitmap, float x1, float x2, float y, COLORREF col);
	void DrawGouraudLine(Bitmap& bitmap, float x1, float x2, float y, COLORREF startingColour, COLORREF endColour);

	void FillBottomFlatTriangle(Vertex v1, Vertex v2, Vertex v3, COLORREF polyColour, Bitmap &bitmap);
	void FillTopFlatTriangle(Vertex v1, Vertex v2, Vertex v3, COLORREF polyColour, Bitmap &bitmap);

	void FillBottomFlatTriangle(Vertex v1, Vertex v2, Vertex v3, Bitmap &bitmap);
	void FillTopFlatTriangle(Vertex v1, Vertex v2, Vertex v3, Bitmap &bitmap);
	void FillPolygonFlat(Vertex v1, Vertex v2, Vertex v3, COLORREF polyColour, Bitmap &bitmap);
	void FillPolygonGouraud(Vertex v1, Vertex v2, Vertex v3, Bitmap &bitmap);

	void FillSolidTextured(Vertex v1, Vertex v2, Vertex v3, Bitmap &bitmap);
	void FillTopTextured(Vertex v1, Vertex v2, Vertex v3, Bitmap &bitmap);
	void FillBottomTextured(Vertex v1, Vertex v2, Vertex v3, Bitmap &bitmap);

	void FillSolidTexturedCorrected(Vertex v1, Vertex v2, Vertex v3, Bitmap &bitmap);
	void FillTopTexturedCorrected(Vertex v1, Vertex v2, Vertex v3, Bitmap &bitmap);
	void FillBottomTexturedCorrected(Vertex v1, Vertex v2, Vertex v3, Bitmap &bitmap);

	// Sets the matrix to transform the model however is specified
	void SetTransformation(Matrix &matrix);

	void DrawString(Bitmap &bitmap, LPCTSTR text);


private:
	unsigned int _x;
	unsigned int _y;
	Model _model;
	Matrix _screen;
	Matrix _persp;
	Camera _cam;
	Vertex _cameraPos;
	bool _firstRun;
	bool _md2Loaded;
	int _rotationAngle;
	Matrix _transformation;
	vector<LightPoint> _pointLights;
	vector<LightDirectional> _directionalLights;
	vector<LightSpot> _spotLights;
	LightAmbient _ambientLight;
	clock_t start;
};

