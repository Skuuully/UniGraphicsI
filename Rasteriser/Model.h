#pragma once
#include "Vertex.h"
#include "Model.h"
#include "Polygon3D.h"
#include "Matrix.h"
#include "Camera.h"
#include "LightAmbient.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"
#include "TexVertex.h"
#include "Texture.h"
#include <vector>
#include <cmath>
using namespace std;

class Model
{
public:
	Model();
	~Model();

	// Method to return the polygons and vertices
	vector<Polygon3D>& GetPolygons();
	vector<Vertex>& GetVertices();
	vector<Vertex>& GetTransformedVertices();
	vector<TexVertex>& GetUVCoords();

	// Returns how many polygons and vertices are in the vectors
	size_t GetPolygonCount() const;
	size_t GetVertexCount() const;
	size_t GetUVCount() const;

	// Methods to add vertices and polygons to their vectors
	void AddVertex(float x, float y, float z);
	void AddPolygon(int i0, int i1, int i2, int uvIndex0, int uvIndex1, int uvIndex2);
	void AddTextureUV(float u, float v);

	// Matrix transforms to transform the vectors
	void TransformLocalVertices(const Matrix &transform);
	void TransformTransformedVertices(const Matrix &transform);

	// Dehomogenizes all of the transformed vertices
	void DehomogenizeTransformedVertices();

	// Marks polygons for culling if they are not to be shown
	void CalculateBackfaces(Camera cam);
	// Sort the polygons based upon their Zdepth
	void Sort();

	// Light calculations for flat shading
	void CalculateLightingDirectional(vector<LightDirectional> directionalLights);
	void CalculateAmbientLight(LightAmbient ambientLight);
	void CalculateLightingPoint(vector<LightPoint> pointLights);

	// Calculate normal vectors for the vertices
	void CalculateNormalVectors();

	// Uses the vertex colours
	void CalculateAmbientLightVertex(LightAmbient ambientLight);
	void CalculateVertexLightingDirectional(vector<LightDirectional> directionalLights);
	void CalculateVertexLightingPoint(vector<LightPoint> pointLights);

	Vector3D CalculateH(Vector3D l, Vector3D v);
	// Also does the specular lighting
	void CalculateDirectionalWithSpecular(vector<LightDirectional> directionalLights, Camera cam);
	void CalculatePointWithSpecular(vector<LightPoint> pointLights, Vertex camPos);
	void CalculateSpotWithSpecular(vector<LightSpot> spotLights, Vertex camPos);

	// Calculates value between 0 and 1 for spot light 
	// edge0 = cos(outer)	edge1 = cos(inner)	x = l.n
	float Smoothstep(float edge0, float edge1, float x);
	// Caps value at 255max and 0 min
	float ClampValue255(float value);
	// Clamps between 1 and 0
	float CoefficientClamper(float value);

	//Accessors for the reflection co-efficients
	float GetRedCoefficient();
	void SetRedCoefficient(float value);
	float GetGreenCoefficient();
	void SetGreenCoefficient(float value);
	float GetBlueCoefficient();
	void SetBlueCoefficient(float value);
	// ABle to set all at once, and return all as a single a COLORREF
	COLORREF GetKs();
	void SetKs(float red,float green, float blue);
	// Accessors for specular co-efficients
	float GetKsRed();
	void SetKsRed(float red);
	float GetKsGreen();
	void SetKsGreen(float green);
	float GetKsBlue();
	void SetKsBlue(float blue);
	// Alpha/ Roughness accessor
	float GetRoughness();
	void SetRoughness(float roughness);
	//Used for md2loader
	Texture& GetTexture();

private:
	vector<Polygon3D> _polygons;
	vector<Vertex> _vertices;
	vector<Vertex> _transformedVertices;
	vector<TexVertex> _uvCoords;
	// Light absorption values, higher reflects more light
	float _kdRed;
	float _kdGreen;
	float _kdBlue;
	// constants for specular lighting
	COLORREF _ks;
	float _ksRed;
	float _ksGreen;
	float _ksBlue;
	// How shiny a model is, 1 is shiny, 0 is dull
	float _roughness;
	Texture _texture;
};

