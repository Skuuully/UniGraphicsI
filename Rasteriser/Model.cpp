#include "Model.h"
#include <algorithm>

Model::Model()
{
}


Model::~Model()
{
}


void Model:: AddVertex(float x, float y, float z)
{
	Vertex vert(x, y, z);
	_vertices.push_back(vert);
}

void Model::AddPolygon(int i0, int i1, int i2, int uvIndex0, int uvIndex1, int uvIndex2)
{
	Polygon3D poly(i0, i1, i2, uvIndex0, uvIndex1, uvIndex2);
	_polygons.push_back(poly);
}

void Model::AddTextureUV(float u, float v)
{
	TexVertex tex(u, v);
	_uvCoords.push_back(tex);
}

vector<Polygon3D> &Model:: GetPolygons()
{
	return _polygons;
}

vector<Vertex> &Model :: GetVertices()
{
	return _vertices;
}

vector<Vertex>& Model::GetTransformedVertices()
{
	return _transformedVertices;
}

vector<TexVertex>& Model::GetUVCoords()
{
	return _uvCoords;
}

size_t Model:: GetPolygonCount() const
{
	int polygonCount = (int)_polygons.size();
	return polygonCount;
}

size_t Model::GetVertexCount() const
{
	int verticesCount = (int)_vertices.size();
	return verticesCount;
}

size_t Model::GetUVCount() const
{
	int uvCount = (int)_uvCoords.size();
	return uvCount;
}

// Use transform local first time so that the original vertices 
// are retained as it is hard to get back to them if required
void Model::TransformLocalVertices(const Matrix &transform)
{
	for (int i = 0; i < _vertices.size(); i++)
	{
		// Clear the vector for the next time this method is called, otherwise vector gets bigger and bigger
		if(_transformedVertices.size() == _vertices.size())
		{
			_transformedVertices.clear();
		}
		_transformedVertices.push_back(transform * _vertices[i]);
	}
}

void Model::TransformTransformedVertices(const Matrix &transform)
{
	for (int i = 0; i < _transformedVertices.size(); i++)
	{
		_transformedVertices[i] = transform * _transformedVertices[i];
	}
}

void Model::DehomogenizeTransformedVertices()
{
	for (int i = 0; i < _transformedVertices.size(); i++)
	{
		_transformedVertices[i] = _transformedVertices[i].Dehomogenize();
	}
}

void Model::CalculateBackfaces(Camera cam)
{
	for (int i = 0; i < _polygons.size(); i++)
	{
		// Gets the vertices relating to each polygon in vertex form
		Vertex vertices[3];
		vertices[0] = _transformedVertices[_polygons[i].GetIndex(0)];
		vertices[1] = _transformedVertices[_polygons[i].GetIndex(1)];
		vertices[2] = _transformedVertices[_polygons[i].GetIndex(2)];

		// gets vectors based on the vertices to create a normal between them
		Vector3D vectA = vertices[0] - vertices[1];
		Vector3D vectB = vertices[0] - vertices[2];
		Vector3D normal = vectA.CrossProduct(vectB, vectA);

		// Store the normal value for later (lighting)
		_polygons[i].SetNormal(normal);

		// calculate the vector between the camera and the normal of the polygon
		// if the angle is less than zero then the polygon does not face towards
		// the camera and therefore want culling
		Vector3D eyeVector = vertices[0] - cam.GetPosition();
		if (normal.DotProduct(normal, eyeVector) < 0)
		{
			_polygons[i].SetCull(true);
		}
		else
		{
			_polygons[i].SetCull(false);
		}
	}
}

// Predicate used for sorting the polygon collection
bool Compare(Polygon3D &poly, Polygon3D &poly2)
{
	return poly.GetDepth() > poly2.GetDepth();
}

void Model::Sort()
{
	// for each polygon
	for (int i = 0; i < _polygons.size(); i++)
	{
		// calculate the average z value between the vertices 
		float avgZDepth = (_transformedVertices[_polygons[i].GetIndex(0)].GetZ() +
		_transformedVertices[_polygons[i].GetIndex(1)].GetZ() +
		 _transformedVertices[_polygons[i].GetIndex(2)].GetZ()) / 3;

		// set the depth of that plane formed by the vertices to this average value
		_polygons[i].SetDepth(avgZDepth);
	}
	sort(_polygons.begin(), _polygons.end(), Compare);
}

void Model::CalculateLightingDirectional(vector<LightDirectional> directionalLights)
{
	// declare floats to store the colour values
	float red = 0, blue = 0, green = 0;
	float totalRed = 0, totalBlue = 0, totalGreen = 0;

	// For each polygon in the model and for each light apply the light calculations
	for (int i = 0; i < GetPolygonCount(); i++)
	{
		// If polygon is marked for cull do not bother calculating what the light will do to polygon
		if (!_polygons[i].GetCull())
		{
			// Reset the total values for this polygon
			totalRed = _polygons[i].GetRed(), totalBlue = _polygons[i].GetBlue(), totalGreen = _polygons[i].GetGreen();
			for (int j = 0; j < directionalLights.size(); j++)
			{
				// Normalise the lights direction vector
				Vector3D normalisedLight = directionalLights[j].GetDirection().Normalise();

				// Calculate the dot product of the normalised light and the normal of the polygon
				float dotProd = _polygons[i].GetNormal().DotProduct(normalisedLight, _polygons[i].GetNormal().Normalise());

				// Multiply light intensity by model reflection and multilpy by dot product
				red = directionalLights[j].GetRedIntensity() * _kdRed * dotProd;
				green = directionalLights[j].GetGreenIntensity() * _kdGreen * dotProd;
				blue = directionalLights[j].GetBlueIntensity() * _kdBlue * dotProd;

				// Add the calculated values to the total
				totalRed += red;
				totalGreen += green;
				totalBlue += blue;
			}
			// Clamping the RGB values
			totalRed = ClampValue255(totalRed);
			totalGreen = ClampValue255(totalGreen);
			totalBlue = ClampValue255(totalBlue);
			_polygons[i].SetRed(totalRed);
			_polygons[i].SetGreen((int)totalGreen);
			_polygons[i].SetBlue((int)totalBlue);
		}
	}
}

void Model::CalculateAmbientLight(LightAmbient ambientLight)
{
	for (int i = 0; i < GetPolygonCount(); i++)
	{

		// If polygon is marked for cull do not bother calculating what the light will do to polygon
		if (!_polygons[i].GetCull())
		{
			// declare floats to store the colour values
			float red = 0, blue = 0, green = 0;

			// Multiply light intensity by model reflection
			red = ambientLight.GetRed() * _kdRed;
			green = ambientLight.GetGreen() * _kdGreen;
			blue = ambientLight.GetBlue() * _kdBlue;

			// Clamping the RGB values
			red = ClampValue255(red);
			green = ClampValue255(green);
			blue = ClampValue255(blue);
			_polygons[i].SetRed(red);
			_polygons[i].SetGreen((const int)green);
			_polygons[i].SetBlue((const int)blue);
		}
	}
}

void Model::CalculateLightingPoint(vector<LightPoint> pointLights)
{
	// declare floats to store the colour values
	float red = 0, blue = 0, green = 0;
	float totalRed = 0, totalBlue = 0, totalGreen = 0;

	// For each polygon in the model and for each light apply the light calculations
	for (int i = 0; i < GetPolygonCount(); i++)
	{
		// If polygon is marked for cull do not bother calculating what the light will do to polygon
		if (!_polygons[i].GetCull())
		{
			// Reset the total values for this polygon
			totalRed = _polygons[i].GetRed(), totalBlue = _polygons[i].GetBlue(), totalGreen = _polygons[i].GetGreen();
			for (int j = 0; j < pointLights.size(); j++)
			{
				// Get vector to the light based on first vertex and normalise
				Vector3D toLight = _transformedVertices[_polygons[i].GetIndex(0)] - pointLights[j].GetPosition();

				float toLightDistance = sqrt(toLight.GetX() * toLight.GetX() + toLight.GetY() * toLight.GetY() + toLight.GetZ() * toLight.GetZ());

				toLight = toLight.Normalise();
				float dotProd = _polygons[i].GetNormal().DotProduct(toLight, _polygons[i].GetNormal().Normalise());

				float attenuation = 1 / (pointLights[j].GetAttenuationA() + (pointLights[j].GetAttenuationB() * toLightDistance) + (pointLights[j].GetAttenuationC() * pow(toLightDistance, 2)));
				// Multiply light intensity by model reflection and multilpy by dot product
				red = pointLights[j].GetRedIntensity()  * _kdRed * attenuation * dotProd;
				green = pointLights[j].GetGreenIntensity()  * _kdGreen * attenuation * dotProd;
				blue = pointLights[j].GetBlueIntensity()  * _kdBlue * attenuation * dotProd;

				// Add the calculated values to the total
				totalRed += red;
				totalGreen += green;
				totalBlue += blue;
			}
			// Clamping the RGB values
			totalRed = ClampValue255(totalRed);
			totalGreen = ClampValue255(totalGreen);
			totalBlue = ClampValue255(totalBlue);
			_polygons[i].SetRed(totalRed);
			_polygons[i].SetGreen((const int)totalGreen);
			_polygons[i].SetBlue((const int)totalBlue);
		}
	}
}

void Model::CalculateNormalVectors()
{
	Vertex vertices[3];

	// Reset the vertices normals and timesused
	size_t verticesSize = _transformedVertices.size();
	for (int i = 0; i < verticesSize; i++)
	{
		_transformedVertices[i].SetNormal(Vector3D(0, 0, 0));
		_transformedVertices[i].SetTimesUsed(0);
	}

	size_t polygonSize = _polygons.size();
	for (int i = 0; i < polygonSize; i++)
	{
		// Get vertices for current poly
		vertices[0] = _transformedVertices[_polygons[i].GetIndex(0)];
		vertices[1] = _transformedVertices[_polygons[i].GetIndex(1)];
		vertices[2] = _transformedVertices[_polygons[i].GetIndex(2)];

		// Add polygon normal to vertice normal
		vertices[0].SetNormal(vertices[0].GetNormal() + _polygons[i].GetNormal());
		vertices[1].SetNormal(vertices[1].GetNormal() + _polygons[i].GetNormal());
		vertices[2].SetNormal(vertices[2].GetNormal() + _polygons[i].GetNormal());

		// Add 1 to vertex times used as it has been used
		vertices[0].SetTimesUsed(vertices[0].GetTimesUsed() + 1);
		vertices[1].SetTimesUsed(vertices[1].GetTimesUsed() + 1);
		vertices[2].SetTimesUsed(vertices[2].GetTimesUsed() + 1);

		// Set the vertices back to the temp vertices
		_transformedVertices[_polygons[i].GetIndex(0)] = vertices[0];
		_transformedVertices[_polygons[i].GetIndex(1)] = vertices[1];
		_transformedVertices[_polygons[i].GetIndex(2)] = vertices[2];
	}

	for (int i = 0; i < verticesSize; i++)
	{
		int timesUsed = _transformedVertices[i].GetTimesUsed();
		Vector3D normal = _transformedVertices[i].GetNormal();
		float normX;
		float normY;
		float normZ;
		if (timesUsed == 1)
		{
			normX = normal.GetX();
			normY = normal.GetY();
			normZ = normal.GetZ();
		}
		else
		{
			normX = normal.GetX() / timesUsed;
			normY = normal.GetY() / timesUsed;
			normZ = normal.GetZ() / timesUsed;
		}

		// Create vector based on floats and normalise this will then be that vertices normal
		Vector3D tempNorm(normX, normY, normZ);
		tempNorm = tempNorm.Normalise();
		_transformedVertices[i].SetNormal(tempNorm);
	}
}

void Model::CalculateAmbientLightVertex(LightAmbient ambientLight)
{
	for (int i = 0; i < GetPolygonCount(); i++)
	{

		// If polygon is marked for cull do not bother calculating what the light will do to polygon
		if (!_polygons[i].GetCull())
		{
			for (int j = 0; j < _transformedVertices.size(); j++)
			{
				// declare floats to store the colour values
				float red = 0, blue = 0, green = 0;

				// Multiply light intensity by model reflection
				red = ambientLight.GetRed() * _kdRed;
				green = ambientLight.GetGreen() * _kdGreen;
				blue = ambientLight.GetBlue() * _kdBlue;

				// Clamping the RGB values
				red = ClampValue255(red);
				green = ClampValue255(green);
				blue = ClampValue255(blue);
				_transformedVertices[j].SetColor(RGB(red, green, blue));
			}
		}
	}
}

void Model::CalculateVertexLightingDirectional(vector<LightDirectional> directionalLights)
{
	// declare floats to store the colour values
	float red = 0, blue = 0, green = 0;
	float totalRed = 0, totalBlue = 0, totalGreen = 0;
	COLORREF color;
	Vertex triangleVertex[3];
	size_t directionalLightsSize = directionalLights.size();

	// For each polygon in the model and for each light apply the light calculations
	for (int i = 0; i < GetPolygonCount(); i++)
	{
		// If polygon is marked for cull do not bother calculating what the light will do to polygon
		if (!_polygons[i].GetCull())
		{
			// Get the vertices of the polygon being shown
			triangleVertex[0] = _transformedVertices[_polygons[i].GetIndex(0)];
			triangleVertex[1] = _transformedVertices[_polygons[i].GetIndex(1)];
			triangleVertex[2] = _transformedVertices[_polygons[i].GetIndex(2)];
			size_t triangleVertexSize = sizeof(triangleVertex) / sizeof(*triangleVertex);


			for (int k = 0; k < triangleVertexSize; k++)
			{
				color = triangleVertex[k].GetColor();
				totalRed = (float)GetRValue(color), totalBlue = (float)GetBValue(color), totalGreen = (float)GetGValue(color);

				for (int j = 0; j < directionalLightsSize; j++)
				{
					// Normalise the lights direction vector
					Vector3D normalisedLight = directionalLights[j].GetDirection().Normalise();

					// Calculate the dot product of the normalised light and the normal of the polygon
					float dotProd = triangleVertex[k].GetNormal().DotProduct(normalisedLight, triangleVertex[k].GetNormal());

					// Multiply light intensity by model reflection and multilpy by dot product
					red = directionalLights[j].GetRedIntensity() * _kdRed * dotProd;
					green = directionalLights[j].GetGreenIntensity() * _kdGreen * dotProd;
					blue = directionalLights[j].GetBlueIntensity() * _kdBlue * dotProd;

					// Add the calculated values to the total
					totalRed += red;
					totalGreen += green;
					totalBlue += blue;
				}
				// Clamping the RGB values
				totalRed = ClampValue255(totalRed);
				totalGreen = ClampValue255(totalGreen);
				totalBlue = ClampValue255(totalBlue);
				triangleVertex[k].SetColor(RGB(totalRed, totalGreen, totalBlue));
			}
			_transformedVertices[_polygons[i].GetIndex(0)] = triangleVertex[0];
			_transformedVertices[_polygons[i].GetIndex(1)] = triangleVertex[1];
			_transformedVertices[_polygons[i].GetIndex(2)] = triangleVertex[2];
		}
	}
}

void Model::CalculateVertexLightingPoint(vector<LightPoint> pointLights)
{
	// declare floats to store the colour values
	float red = 0, blue = 0, green = 0;
	float totalRed = 0, totalBlue = 0, totalGreen = 0;
	Vertex triangleVertex[3];

	// For each polygon in the model and for each light apply the light calculations
	for (int i = 0; i < GetPolygonCount(); i++)
	{
		// If polygon is marked for cull do not bother calculating what the light will do to polygon
		if (!_polygons[i].GetCull())
		{
			triangleVertex[0] = _transformedVertices[_polygons[i].GetIndex(0)];
			triangleVertex[1] = _transformedVertices[_polygons[i].GetIndex(1)];
			triangleVertex[2] = _transformedVertices[_polygons[i].GetIndex(2)];
			size_t triangleVertexSize = sizeof(triangleVertex) / sizeof(*triangleVertex);

			for (int k = 0; k < triangleVertexSize; k++)
			{
				// Reset the total values for this vertex
				COLORREF color = triangleVertex[k].GetColor();
				totalRed = GetRValue(color), totalBlue = GetBValue(color), totalGreen = GetGValue(color);

				for (int j = 0; j < pointLights.size(); j++)
				{
					// Get vector to the light based on first vertex and normalise
					Vector3D toLight = triangleVertex[k] - pointLights[j].GetPosition();

					float toLightDistance = sqrt(toLight.GetX() * toLight.GetX() + toLight.GetY() * toLight.GetY() + toLight.GetZ() * toLight.GetZ());

					toLight = toLight.Normalise();
					float dotProd = triangleVertex[k].GetNormal().DotProduct(toLight, triangleVertex[k].GetNormal().Normalise());

					float attenuation = 1 / (pointLights[j].GetAttenuationA() + (pointLights[j].GetAttenuationB() * toLightDistance) + (pointLights[j].GetAttenuationC() * pow(toLightDistance, 2)));
					// Multiply light intensity by model reflection and multilpy by dot product
					red = pointLights[j].GetRedIntensity()  * _kdRed * attenuation * dotProd;
					green = pointLights[j].GetGreenIntensity()  * _kdGreen * attenuation * dotProd;
					blue = pointLights[j].GetBlueIntensity()  * _kdBlue * attenuation * dotProd;

					// Add the calculated values to the total
					totalRed += red;
					totalGreen += green;
					totalBlue += blue;
				}
				// Clamping the RGB values
				totalRed = ClampValue255(totalRed);
				totalGreen = ClampValue255(totalGreen);
				totalBlue = ClampValue255(totalBlue);
				triangleVertex[k].SetColor(RGB(totalRed, totalGreen, totalBlue));
			}

			_transformedVertices[_polygons[i].GetIndex(0)] = triangleVertex[0];
			_transformedVertices[_polygons[i].GetIndex(1)] = triangleVertex[1];
			_transformedVertices[_polygons[i].GetIndex(2)] = triangleVertex[2];
		}
	}
}

Vector3D Model::CalculateH(Vector3D l, Vector3D v)
{
	Vector3D lplusv(l.GetX() + v.GetX(), l.GetY() + v.GetY(), l.GetZ() + v.GetZ());
	float lvx = lplusv.GetX();
	float lvy = lplusv.GetY();
	float lvz = lplusv.GetZ();

	float hx = lvx / (lvx * lvx);
	float hy = lvy / (lvy * lvy);
	float hz = lvz / (lvz * lvz);
	Vector3D h(hx, hy, hz);
	return h;
}

void Model::CalculateDirectionalWithSpecular(vector<LightDirectional> directionalLights, Camera cam)
{	
	float red = 0, blue = 0, green = 0;
	float totalRed = 0, totalBlue = 0, totalGreen = 0;
	COLORREF color;
	Vertex triangleVertex[3];
	size_t directionalLightsSize = directionalLights.size();

	// For each polygon in the model and for each light apply the light calculations
	for (int i = 0; i < GetPolygonCount(); i++)
	{
		// If polygon is marked for cull do not bother calculating what the light will do to polygon
		if (!_polygons[i].GetCull())
		{
			// Get the vertices of the polygon being shown
			triangleVertex[0] = _transformedVertices[_polygons[i].GetIndex(0)];
			triangleVertex[1] = _transformedVertices[_polygons[i].GetIndex(1)];
			triangleVertex[2] = _transformedVertices[_polygons[i].GetIndex(2)];
			size_t triangleVertexSize = sizeof(triangleVertex) / sizeof(*triangleVertex);


			for (int k = 0; k < triangleVertexSize; k++)
			{
				color = triangleVertex[k].GetColor();
				totalRed = (float)GetRValue(color), totalBlue = (float)GetBValue(color), totalGreen = (float)GetGValue(color);

				for (int j = 0; j < directionalLightsSize; j++)
				{
					// Normalise the lights direction vector
					Vector3D normalisedLight = directionalLights[j].GetDirection().Normalise();

					// Calculate the dot product of the normalised light and the normal of the vertex
					float dotProd = triangleVertex[k].GetNormal().DotProduct(normalisedLight, triangleVertex[k].GetNormal());

					// Multiply light intensity by model reflection and multilpy by dot product
					float kdRed = directionalLights[j].GetRedIntensity() * _kdRed * dotProd;
					float kdGreen = directionalLights[j].GetGreenIntensity() * _kdGreen * dotProd;
					float kdBlue = directionalLights[j].GetBlueIntensity() * _kdBlue * dotProd;

					// Work out h where l is direction to light and v is direction to camera
					Vector3D l = (directionalLights[j].GetDirection());
					l.Invert();
					Vector3D v = cam.GetPosition() - triangleVertex[k];
					Vector3D h = CalculateH(l, v);

					dotProd = triangleVertex[k].GetNormal().DotProduct(triangleVertex[k].GetNormal(), h);
					float normalPowerA = pow(dotProd, _roughness);
					// ks vals = ksI(N.H)^a
					float ksRed = _ksRed * directionalLights[j].GetRedIntensity() * normalPowerA;
					float ksGreen = _ksGreen * directionalLights[j].GetGreenIntensity() * normalPowerA;
					float ksBlue = _ksBlue * directionalLights[j].GetBlueIntensity() * normalPowerA;
					// add together the diffuse and specular counterparts
					red = kdRed + ksRed;
					green = kdGreen + ksGreen;
					blue = kdBlue + ksBlue;

					// Add the calculated values to the total
					totalRed += red;
					totalGreen += green;
					totalBlue += blue;
				}
				// Clamping the RGB values
				totalRed = ClampValue255(totalRed);
				totalGreen = ClampValue255(totalGreen);
				totalBlue = ClampValue255(totalBlue);
				triangleVertex[k].SetColor(RGB(totalRed, totalGreen, totalBlue));
			}
			_transformedVertices[_polygons[i].GetIndex(0)] = triangleVertex[0];
			_transformedVertices[_polygons[i].GetIndex(1)] = triangleVertex[1];
			_transformedVertices[_polygons[i].GetIndex(2)] = triangleVertex[2];
		}
	}
}

void Model::CalculatePointWithSpecular(vector<LightPoint> pointLights, Vertex camPos)
{
	// declare floats to store the colour values
	float red = 0, blue = 0, green = 0;
	float totalRed = 0, totalBlue = 0, totalGreen = 0;
	Vertex triangleVertex[3];

	// For each polygon in the model and for each light apply the light calculations
	for (int i = 0; i < GetPolygonCount(); i++)
	{
		// If polygon is marked for cull do not bother calculating what the light will do to polygon
		if (!_polygons[i].GetCull())
		{
			triangleVertex[0] = _transformedVertices[_polygons[i].GetIndex(0)];
			triangleVertex[1] = _transformedVertices[_polygons[i].GetIndex(1)];
			triangleVertex[2] = _transformedVertices[_polygons[i].GetIndex(2)];
			size_t triangleVertexSize = sizeof(triangleVertex) / sizeof(*triangleVertex);

			for (int k = 0; k < triangleVertexSize; k++)
			{
				// Reset the total values for this vertex
				COLORREF color = triangleVertex[k].GetColor();
				totalRed = GetRValue(color), totalBlue = GetBValue(color), totalGreen = GetGValue(color);

				for (int j = 0; j < pointLights.size(); j++)
				{
					// Get vector to the light based on first vertex and normalise
					Vector3D toLight = triangleVertex[k] - pointLights[j].GetPosition();

					float toLightDistance = sqrt(toLight.GetX() * toLight.GetX() + toLight.GetY() * toLight.GetY() + toLight.GetZ() * toLight.GetZ());

					toLight = toLight.Normalise();
					float dotProd = triangleVertex[k].GetNormal().DotProduct(toLight, triangleVertex[k].GetNormal().Normalise());

					float attenuation = 1 / (pointLights[j].GetAttenuationA() + (pointLights[j].GetAttenuationB() * toLightDistance) + (pointLights[j].GetAttenuationC() * pow(toLightDistance, 2)));

					float kdRed = pointLights[j].GetRedIntensity()  * _kdRed * attenuation * dotProd;
					float kdGreen = pointLights[j].GetGreenIntensity()  * _kdGreen * attenuation * dotProd;
					float kdBlue = pointLights[j].GetBlueIntensity()  * _kdBlue * attenuation * dotProd;

					// Work out h where l is direction to light and v is direction to camera
					Vector3D l = pointLights[j].GetPosition() - triangleVertex[k];
					l.Invert();
					Vector3D v = camPos - triangleVertex[k];
					Vector3D h = CalculateH(l, v);

					dotProd = triangleVertex[k].GetNormal().DotProduct(triangleVertex[k].GetNormal(), h);
					float normalPowerA = pow(dotProd, _roughness);
					// ks vals = ksI(N.H)^a * atten
					float ksRed = _ksRed * pointLights[j].GetRedIntensity() * normalPowerA * attenuation;
					float ksGreen = _ksGreen * pointLights[j].GetGreenIntensity() * normalPowerA * attenuation;
					float ksBlue = _ksBlue * pointLights[j].GetBlueIntensity() * normalPowerA * attenuation;

					// Multiply light intensity by model reflection and multilpy by dot product
					red = kdRed + ksRed;
					green = kdGreen + ksGreen;
					blue = kdBlue + ksBlue;

					// Add the calculated values to the total
					totalRed += red;
					totalGreen += green;
					totalBlue += blue;
				}
				// Clamping the RGB values
				totalRed = ClampValue255(totalRed);
				totalGreen = ClampValue255(totalGreen);
				totalBlue = ClampValue255(totalBlue);
				triangleVertex[k].SetColor(RGB(totalRed, totalGreen, totalBlue));
			}

			_transformedVertices[_polygons[i].GetIndex(0)] = triangleVertex[0];
			_transformedVertices[_polygons[i].GetIndex(1)] = triangleVertex[1];
			_transformedVertices[_polygons[i].GetIndex(2)] = triangleVertex[2];
		}
	}
}

void Model::CalculateSpotWithSpecular(vector<LightSpot> spotLights, Vertex camPos)
{
	// declare floats to store the colour values
	float red = 0, blue = 0, green = 0;
	float totalRed = 0, totalBlue = 0, totalGreen = 0;
	Vertex triangleVertex[3];

	// For each polygon in the model and for each light apply the light calculations
	for (int i = 0; i < GetPolygonCount(); i++)
	{
		// If polygon is marked for cull do not bother calculating what the light will do to polygon
		if (!_polygons[i].GetCull())
		{
			triangleVertex[0] = _transformedVertices[_polygons[i].GetIndex(0)];
			triangleVertex[1] = _transformedVertices[_polygons[i].GetIndex(1)];
			triangleVertex[2] = _transformedVertices[_polygons[i].GetIndex(2)];
			size_t triangleVertexSize = sizeof(triangleVertex) / sizeof(*triangleVertex);

			for (int k = 0; k < triangleVertexSize; k++)
			{
				// Reset the total values for this vertex
				COLORREF color = triangleVertex[k].GetColor();
				totalRed = GetRValue(color), totalBlue = GetBValue(color), totalGreen = GetGValue(color);

				for (int j = 0; j < spotLights.size(); j++)
				{
					// Get vector to the light based on first vertex and normalise
					Vector3D toLight = triangleVertex[k] - spotLights[j].GetPosition();

					float toLightDistance = sqrt(toLight.GetX() * toLight.GetX() + toLight.GetY() * toLight.GetY() + toLight.GetZ() * toLight.GetZ());

					toLight = toLight.Normalise();
					float dotProd = triangleVertex[k].GetNormal().DotProduct(toLight, triangleVertex[k].GetNormal().Normalise());

					float attenuation = 1 / (spotLights[j].GetAttenuationA() + (spotLights[j].GetAttenuationB() * toLightDistance) + (spotLights[j].GetAttenuationC() * pow(toLightDistance, 2)));

					float convert = 3.14159f / 180;
					float cosOuter = (float)cos((double)spotLights[j].GetOuter() * convert);
					float cosInner = (float)cos((double)spotLights[j].GetInner() * convert);
					float smoothness = Smoothstep(cosOuter, cosInner, dotProd);

					float kdRed = spotLights[j].GetRedIntensity()  * _kdRed * attenuation * dotProd * smoothness;
					float kdGreen = spotLights[j].GetGreenIntensity()  * _kdGreen * attenuation * dotProd * smoothness;
					float kdBlue = spotLights[j].GetBlueIntensity()  * _kdBlue * attenuation * dotProd * smoothness;

					// Work out h where l is direction to light and v is direction to camera
					Vector3D l = spotLights[j].GetPosition() - triangleVertex[k];
					//l.Invert();
					Vector3D v = camPos - triangleVertex[k];
					Vector3D h = CalculateH(l, v);

					dotProd = triangleVertex[k].GetNormal().DotProduct(triangleVertex[k].GetNormal(), h);
					float normalPowerA = pow(dotProd, _roughness);
					// ks vals = ksI(N.H)^a * atten
					float ksRed = _ksRed * spotLights[j].GetRedIntensity() * normalPowerA * attenuation * smoothness;
					float ksGreen = _ksGreen * spotLights[j].GetGreenIntensity() * normalPowerA * attenuation * smoothness;
					float ksBlue = _ksBlue * spotLights[j].GetBlueIntensity() * normalPowerA * attenuation * smoothness;

					// Multiply light intensity by model reflection and multilpy by dot product
					red = kdRed + ksRed;
					green = kdGreen + ksGreen;
					blue = kdBlue + ksBlue;

					// Add the calculated values to the total
					totalRed += red;
					totalGreen += green;
					totalBlue += blue;
				}
				// Clamping the RGB values
				totalRed = ClampValue255(totalRed);
				totalGreen = ClampValue255(totalGreen);
				totalBlue = ClampValue255(totalBlue);
				triangleVertex[k].SetColor(RGB(totalRed, totalGreen, totalBlue));
			}

			_transformedVertices[_polygons[i].GetIndex(0)] = triangleVertex[0];
			_transformedVertices[_polygons[i].GetIndex(1)] = triangleVertex[1];
			_transformedVertices[_polygons[i].GetIndex(2)] = triangleVertex[2];
		}
	}
}

float Model::Smoothstep(float edge0, float edge1, float x)
{
	float result;
	if (x < edge0)
	{
		result = 0.0f;
	}
	else if (x >= edge1)
	{
		result = 1.0f;
	}
	else
	{
		float t = (x - edge0) / (edge1 - edge0);
		result = (3.0f - 2.0f*t)*(t*t);
	}
	return result;
}

float Model::ClampValue255(float value)
{
	if (value > 255)
	{
		value = 255;
	}
	else if (value < 0)
	{
		value = 0;
	}
	return value;
}

float Model::CoefficientClamper(float value)
{
	if (value > 1)
	{
		return 1;
	}
	else if (value < 0)
	{
		return 0;
	}
	else
	{
		return value;
	}
}

float Model::GetRedCoefficient()
{
	return _kdBlue;
}

void Model::SetRedCoefficient(float value)
{
	if (value > 1)
	{
		_kdRed = 1.0f;
	}
	else if (value < 0)
	{
		_kdRed = 0;
	}
	else
	{
		_kdRed = value;
	}

}

float Model::GetGreenCoefficient()
{
	return _kdGreen;
}

void Model::SetGreenCoefficient(float value)
{
	if (value > 1)
	{
		_kdBlue = 1.0f;
	}
	else if (value < 0)
	{
		_kdBlue = 0;
	}
	else
	{
		_kdBlue = value;
	}
}

float Model::GetBlueCoefficient()
{
	return _kdBlue;
}

void Model::SetBlueCoefficient(float value)
{
	if (value > 1)
	{
		_kdGreen = 1.0f;
	}
	else if (value < 0)
	{
		_kdGreen = 0;
	}
	else
	{
		_kdGreen = value;
	}
}

COLORREF Model::GetKs()
{
	return _ks;
}

void Model::SetKs(float red, float green, float blue)
{
	_ks = RGB(CoefficientClamper(red), CoefficientClamper(green), CoefficientClamper(blue));
}

float Model::GetKsRed()
{
	return _ksRed;
}

void Model::SetKsRed(float red)
{
	_ksRed = CoefficientClamper(red);
}

float Model::GetKsGreen()
{
	return _ksGreen;
}

void Model::SetKsGreen(float green)
{
	_ksGreen = CoefficientClamper(green);
}

float Model::GetKsBlue()
{
	return _ksBlue;
}

void Model::SetKsBlue(float blue)
{
	_ksBlue = CoefficientClamper(blue);
}

float Model::GetRoughness()
{
	return _roughness;
}

void Model::SetRoughness(float roughness)
{
	_roughness = roughness;
}

Texture & Model::GetTexture()
{
	return _texture;
}

