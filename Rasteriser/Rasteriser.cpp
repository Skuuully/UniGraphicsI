#include "Rasteriser.h"

Rasteriser app;

bool Rasteriser::Initialise()
{
	// Camera position initialisng to create the camera matrix, creted everytime a set method is called
	// as well as setting md2 loaded to false
	// MD2loaded stops code from running if the the model is not loaded due to 
	// vector subscript out of range errors otherwise

	_cameraPos.SetX(0);
	_cameraPos.SetY(0);
	_cameraPos.SetZ(-50);
	_cam.SetPosition(_cameraPos);
	_cam.SetxRotation(0);
	_cam.SetyRotation(0);
	_cam.SetzRotation(0);

	Vector3D direction(-50, 0, 50);
	LightDirectional directionalLight(100, 155, 100, direction);
	_directionalLights.push_back(directionalLight);

	Vertex position(0, 0, -50);
	LightPoint pointLight(155, 155, 53, position, 0, 0.1f, 0);
	_pointLights.push_back(pointLight);

	LightSpot spotLight(200, 200, 200, position, 0, 0.1f, 0, 10, 20);
	_spotLights.push_back(spotLight);

	_ambientLight.SetRed(50);
	_ambientLight.SetGreen(50);
	_ambientLight.SetBlue(50);

	//_firstRun = true;
	_md2Loaded = false;

	// Load the model from file and change sate of MD2Loaded to reflect if loaded or not
	 //Use null for 2nd attribute to not load a texture
	if (!MD2Loader::LoadModel("..\\MD2\\cube.md2", "..\\Textures\\lines.pcx", _model,
		&Model::AddPolygon,
		&Model::AddVertex,
		&Model::AddTextureUV))
	{
		Sleep(2000);
		return false;
	}
	else
	{
		_md2Loaded = true;
	}

	_model.SetRedCoefficient(0.5);
	_model.SetGreenCoefficient(0.5);
	_model.SetBlueCoefficient(0.5);

	_model.SetKsRed(0.2f);
	_model.SetKsGreen(0.2f);
	_model.SetKsBlue(0.2f);

	_model.SetRoughness(1);

	start = clock();
	// Make sure you return true unless there were any errors
	// in this method
	return true;
}

void Rasteriser::Update(Bitmap &bitmap)
{

	// Allows the model to spin around the x, y, or z axes
	SetTransformation(_transformation.YRotationMatrix((float)_rotationAngle));
	_rotationAngle++;


	// Generate the matrices in case the window is resized
	GeneratePerspectiveMatrix(1.0f, (float)bitmap.GetWidth() / (float)bitmap.GetHeight());
	GenerateScreenMatrix(1.0f, bitmap.GetWidth(), bitmap.GetHeight());
}

void Rasteriser::Render(Bitmap &bitmap)
{
	// Clear the bitmap to black
	bitmap.Clear(RGB(50, 50, 50));
	// Graphics pipeline:
	// Model transform ------> Viewing Transform -------> Projection (Perspective) Transform--->
	// <-----------------------------------------------------<
	// -------> Dehomogenize -------> Screen Transform
	if (_md2Loaded)
	{
		// time since started running
		float timeElapsed = (float)(clock() - start) / CLOCKS_PER_SEC;

		/*if (timeElapsed < 5.0f)
		{
			_model.TransformLocalVertices(_transformation);
			if (timeElapsed > 2.5f)
			{
				DrawString(bitmap, L"With backface culling");
				_model.CalculateBackfaces(_cam);
			}
			else
			{
				DrawString(bitmap, L"Plain wireframe");
			}
			_model.CalculateNormalVectors();

			_model.TransformTransformedVertices(_cam.GetViewMatrix());
			_model.Sort();
			_model.TransformTransformedVertices(_persp);
			_model.DehomogenizeTransformedVertices();
			_model.TransformTransformedVertices(_screen);

			DrawWireFrame(bitmap);
		}
		else if (timeElapsed < 7.5f)
		{
			DrawString(bitmap, L"With ambient light + sort");
			_model.TransformLocalVertices(_transformation);
			_model.CalculateBackfaces(_cam);
			_model.CalculateNormalVectors();

			_model.CalculateAmbientLight(_ambientLight);

			_model.TransformTransformedVertices(_cam.GetViewMatrix());
			_model.Sort();
			_model.TransformTransformedVertices(_persp);
			_model.DehomogenizeTransformedVertices();
			_model.TransformTransformedVertices(_screen);

			DrawSolidFlat(bitmap);
		}
		else if (timeElapsed < 10.0f)
		{
			DrawString(bitmap, L"directional + ambient");
			_model.TransformLocalVertices(_transformation);
			_model.CalculateBackfaces(_cam);
			_model.CalculateNormalVectors();

			_model.CalculateAmbientLight(_ambientLight);
			_model.CalculateLightingDirectional(_directionalLights);

			_model.TransformTransformedVertices(_cam.GetViewMatrix());
			_model.Sort();
			_model.TransformTransformedVertices(_persp);
			_model.DehomogenizeTransformedVertices();
			_model.TransformTransformedVertices(_screen);

			DrawSolidFlat(bitmap);
		}
		else if (timeElapsed < 12.5f)
		{
			DrawString(bitmap, L"point + ambient");
			_model.TransformLocalVertices(_transformation);
			_model.CalculateBackfaces(_cam);
			_model.CalculateNormalVectors();

			_model.CalculateAmbientLight(_ambientLight);
			_model.CalculateLightingPoint(_pointLights);

			_model.TransformTransformedVertices(_cam.GetViewMatrix());
			_model.Sort();
			_model.TransformTransformedVertices(_persp);
			_model.DehomogenizeTransformedVertices();
			_model.TransformTransformedVertices(_screen);

			DrawSolidFlat(bitmap);
		}
		else if (timeElapsed < 15.0f)
		{
			DrawString(bitmap, L"all three");
			_model.TransformLocalVertices(_transformation);
			_model.CalculateBackfaces(_cam);
			_model.CalculateNormalVectors();

			_model.CalculateAmbientLight(_ambientLight);
			_model.CalculateLightingDirectional(_directionalLights);
			_model.CalculateLightingPoint(_pointLights);

			_model.TransformTransformedVertices(_cam.GetViewMatrix());
			_model.Sort();
			_model.TransformTransformedVertices(_persp);
			_model.DehomogenizeTransformedVertices();
			_model.TransformTransformedVertices(_screen);

			DrawSolidFlat(bitmap);
		}
		else if (timeElapsed < 18.0f)
		{
			DrawString(bitmap, L"My rasterisation code with all three");
			_model.TransformLocalVertices(_transformation);
			_model.CalculateBackfaces(_cam);
			_model.CalculateNormalVectors();

			_model.CalculateAmbientLight(_ambientLight);
			_model.CalculateLightingDirectional(_directionalLights);
			_model.CalculateLightingPoint(_pointLights);

			_model.TransformTransformedVertices(_cam.GetViewMatrix());
			_model.Sort();
			_model.TransformTransformedVertices(_persp);
			_model.DehomogenizeTransformedVertices();
			_model.TransformTransformedVertices(_screen);

			MyDrawSolidFlat(bitmap);
		}
		else if (timeElapsed < 23.0f)
		{
			DrawString(bitmap, L"Gouraud shading");
			_model.TransformLocalVertices(_transformation);
			_model.CalculateBackfaces(_cam);
			_model.CalculateNormalVectors();

			_model.CalculateAmbientLightVertex(_ambientLight);
			_model.CalculateVertexLightingDirectional(_directionalLights);
			_model.CalculateVertexLightingPoint(_pointLights);
	
			_model.TransformTransformedVertices(_cam.GetViewMatrix());
			_model.Sort();
			_model.TransformTransformedVertices(_persp);
			_model.DehomogenizeTransformedVertices();
			_model.TransformTransformedVertices(_screen);

			DrawGouraud(bitmap);
		}
		else if (timeElapsed < 28.0f)
		{
			DrawString(bitmap, L"With specular");
			_model.TransformLocalVertices(_transformation);
			_model.CalculateBackfaces(_cam);
			_model.CalculateNormalVectors();

			_model.CalculateAmbientLightVertex(_ambientLight);
			_model.CalculateDirectionalWithSpecular(_directionalLights, _cam);
			_model.CalculatePointWithSpecular(_pointLights, _cameraPos);

			_model.TransformTransformedVertices(_cam.GetViewMatrix());
			_model.Sort();
			_model.TransformTransformedVertices(_persp);
			_model.DehomogenizeTransformedVertices();
			_model.TransformTransformedVertices(_screen);

			DrawGouraud(bitmap);
		}
		else if (timeElapsed < 33.0f)
		{
			DrawString(bitmap, L"Ambient + spot");
			_model.TransformLocalVertices(_transformation);
			_model.CalculateBackfaces(_cam);
			_model.CalculateNormalVectors();

			_model.CalculateAmbientLightVertex(_ambientLight);
			_model.CalculateSpotWithSpecular(_spotLights, _cameraPos);

			_model.TransformTransformedVertices(_cam.GetViewMatrix());
			_model.Sort();
			_model.TransformTransformedVertices(_persp);
			_model.DehomogenizeTransformedVertices();
			_model.TransformTransformedVertices(_screen);

			DrawGouraud(bitmap);
		}
		else if (timeElapsed < 38.0f)
		{
			DrawString(bitmap, L"With texture");
			_model.TransformLocalVertices(_transformation);
			_model.CalculateBackfaces(_cam);
			_model.CalculateNormalVectors();

			_model.CalculateAmbientLightVertex(_ambientLight);
			_model.CalculateDirectionalWithSpecular(_directionalLights, _cam);

			_model.TransformTransformedVertices(_cam.GetViewMatrix());
			_model.Sort();
			_model.TransformTransformedVertices(_persp);
			_model.DehomogenizeTransformedVertices();
			_model.TransformTransformedVertices(_screen);

			DrawSolidTextured(bitmap);
		}
		else
		{
			DrawString(bitmap, L"With perspective correction");
			_model.TransformLocalVertices(_transformation);
			_model.CalculateBackfaces(_cam);
			_model.CalculateNormalVectors();

			_model.CalculateAmbientLightVertex(_ambientLight);
			_model.CalculateDirectionalWithSpecular(_directionalLights, _cam);

			_model.TransformTransformedVertices(_cam.GetViewMatrix());
			_model.Sort();
			_model.TransformTransformedVertices(_persp);
			_model.DehomogenizeTransformedVertices();
			_model.TransformTransformedVertices(_screen);

			DrawSolidTexturedCorrected(bitmap);
		} */

		//All possible functions for model and drawing

		//_model.TransformLocalVertices(_transformation);
		//_model.CalculateBackfaces(_cam);
		//_model.CalculateNormalVectors();

		//_model.CalculateAmbientLight(_ambientLight);
		//_model.CalculateLightingDirectional(_directionalLights);
		//_model.CalculateLightingPoint(_pointLights);
		//_model.CalculateAmbientLightVertex(_ambientLight);
		//_model.CalculateVertexLightingDirectional(_directionalLights);
		//_model.CalculateVertexLightingPoint(_pointLights);
		//_model.CalculateDirectionalWithSpecular(_directionalLights, _cam);
		//_model.CalculatePointWithSpecular(_pointLights, _cameraPos);
		//_model.CalculateSpotWithSpecular(_spotLights, _cameraPos);

		//_model.Sort();
		//_model.TransformTransformedVertices(_persp);
		//_model.DehomogenizeTransformedVertices();
		//_model.TransformTransformedVertices(_screen);

		//DrawWireFrame(bitmap);
		//DrawSolidFlat(bitmap);
		//MyDrawSolidFlat(bitmap);
		//DrawGouraud(bitmap);
		//DrawSolidTextured(bitmap);
		//DrawSolidTexturedCorrected(bitmap);

		//Test area

		_model.TransformLocalVertices(_transformation);
		_model.CalculateBackfaces(_cam);
		_model.CalculateNormalVectors();

		_model.CalculateAmbientLightVertex(_ambientLight);
		_model.CalculateDirectionalWithSpecular(_directionalLights, _cam);
		//_model.CalculateVertexLightingDirectional(_directionalLights);
		_model.CalculateSpotWithSpecular(_spotLights, _cameraPos);

		_model.TransformTransformedVertices(_cam.GetViewMatrix());
		_model.Sort();
		_model.TransformTransformedVertices(_persp);
		_model.DehomogenizeTransformedVertices();
		_model.TransformTransformedVertices(_screen);

		DrawSolidTexturedCorrected(bitmap);
	}
}

void Rasteriser::GeneratePerspectiveMatrix(float d, float aspectRatio)
{
	// Create a new matrix then set the private matrix to this matrix
	// d = 1 for a FOV of 90
	Matrix p{ d / aspectRatio, 0, 0, 0,
			  0, 			   d, 0, 0,
			  0,			   0, d, 0,
			  0,			   0, 1, 0 };
	_persp = p;
}

void Rasteriser::GenerateScreenMatrix(float d, int width, int height)
{
	// Create a new matrix then set the private matrix to this matrix
	Matrix s{ (width / 2.0f),	0,					0,				(width / 2.0f),
				0,				(-height / 2.0f),   0,				(height / 2.0f),
				0,				0,					d / 2.0f,		d / 2.0f,
				0,				0,					0,				1 };
	_screen = s;
}

void Rasteriser::DrawWireFrame(Bitmap & bitmap)
{
	vector<Polygon3D> polys = _model.GetPolygons();
	vector<Vertex> vertices = _model.GetTransformedVertices();
	// Vertices used to draw between
	Vertex triangles[3];

	for(int i = 0; i < _model.GetPolygonCount(); i++)
	{
		// If the polygon is not marked for culling then go through the
		// methods to draw it
		if (!polys[i].GetCull())
		{
			// Sets the points of the triangle to the vertices pointed to by the polygon indexer
			triangles[0] = vertices[polys[i].GetIndex(0)];
			triangles[1] = vertices[polys[i].GetIndex(1)];
			triangles[2] = vertices[polys[i].GetIndex(2)];

			// Draws the lines between the points
			MoveToEx(bitmap.GetDC(), (int)triangles[0].GetX(), (int)triangles[0].GetY(), NULL);
			LineTo(bitmap.GetDC(), (int)triangles[1].GetX(), (int)triangles[1].GetY());
			LineTo(bitmap.GetDC(), (int)triangles[2].GetX(), (int)triangles[2].GetY());
			LineTo(bitmap.GetDC(), (int)triangles[0].GetX(), (int)triangles[0].GetY());
		}
	}
}

void Rasteriser::DrawSolidFlat(Bitmap & bitmap)
{
	// Get the polygons and the vertices of the model
	vector<Polygon3D> polys = _model.GetPolygons();
	vector<Vertex> vertices = _model.GetTransformedVertices();
	POINT pts[3];

	// Iterate through the polygons setting POINT values
	// to then draw using the polygon function
	for (int i = 0; i < _model.GetPolygonCount(); i++)
	{
		if (!polys[i].GetCull())
		{
			// Set the brush, and pen to fill the polygons with
			HBRUSH brush = CreateSolidBrush(RGB(polys[i].GetRed(), polys[i].GetGreen(), polys[i].GetBlue()));
			HPEN pen = CreatePen(1, 2, RGB(polys[i].GetRed(), polys[i].GetGreen(), polys[i].GetBlue()));
			//HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
			//HPEN pen = CreatePen(1, 2, RGB(0, 0, 0));
			HGDIOBJ oldBrush = SelectObject(bitmap.GetDC(), brush);
			HGDIOBJ oldPen = SelectObject(bitmap.GetDC(), pen);

			pts[0].x = (LONG)vertices[polys[i].GetIndex(0)].GetX();
			pts[0].y = (LONG)vertices[polys[i].GetIndex(0)].GetY();
			pts[1].x = (LONG)vertices[polys[i].GetIndex(1)].GetX();
			pts[1].y = (LONG)vertices[polys[i].GetIndex(1)].GetY();
			pts[2].x = (LONG)vertices[polys[i].GetIndex(2)].GetX();
			pts[2].y = (LONG)vertices[polys[i].GetIndex(2)].GetY();
			Polygon(bitmap.GetDC(), pts, 3);
			// Remember to delete the objects afterwards
			SelectObject(bitmap.GetDC(), oldBrush);
			DeleteObject(brush);
			SelectObject(bitmap.GetDC(), oldPen);
			DeleteObject(pen);
		}
	}
}

void Rasteriser::MyDrawSolidFlat(Bitmap & bitmap)
{
	// Get the polygons and the vertices of the model
	vector<Polygon3D> polys = _model.GetPolygons();
	vector<Vertex> vertices = _model.GetTransformedVertices();
	Vertex triangles[3];

	// Iterate through the polygons setting POINT values
	// to then draw using the polygon function
	for (int i = 0; i < _model.GetPolygonCount(); i++)
	{
		if (!polys[i].GetCull())
		{
			// Set the brush, and pen to fill the polygons with
			HBRUSH brush = CreateSolidBrush(RGB(polys[i].GetRed(), polys[i].GetGreen(), polys[i].GetBlue()));
			HPEN pen = CreatePen(1, 2, RGB(polys[i].GetRed(), polys[i].GetGreen(), polys[i].GetBlue()));
			HGDIOBJ oldBrush = SelectObject(bitmap.GetDC(), brush);
			HGDIOBJ oldPen = SelectObject(bitmap.GetDC(), pen);

			triangles[0] = vertices[polys[i].GetIndex(0)];
			triangles[1] = vertices[polys[i].GetIndex(1)];
			triangles[2] = vertices[polys[i].GetIndex(2)];

			COLORREF colour(RGB(polys[i].GetRed(), polys[i].GetGreen(), polys[i].GetBlue()));
			FillPolygonFlat(triangles[0], triangles[1], triangles[2], colour, bitmap);

			// Remember to delete the objects afterwards
			SelectObject(bitmap.GetDC(), oldBrush);
			DeleteObject(brush);
			SelectObject(bitmap.GetDC(), oldPen);
			DeleteObject(pen);
		}
	}
}

void Rasteriser::DrawGouraud(Bitmap & bitmap)
{
	// Get the polygons and the vertices of the model
	vector<Polygon3D> polys = _model.GetPolygons();
	vector<Vertex> vertices = _model.GetTransformedVertices();
	Vertex triangles[3];

	for (int i = 0; i < _model.GetPolygonCount(); i++)
	{
		if (!polys[i].GetCull())
		{
			triangles[0] = vertices[polys[i].GetIndex(0)];
			triangles[1] = vertices[polys[i].GetIndex(1)];
			triangles[2] = vertices[polys[i].GetIndex(2)];


			FillPolygonGouraud(triangles[0], triangles[1], triangles[2], bitmap);
		}
	}
}

void Rasteriser::DrawSolidTextured(Bitmap & bitmap)
{
	// Get the polygons, vertices of the model
	vector<Polygon3D> polys = _model.GetPolygons();
	vector<Vertex> vertices = _model.GetTransformedVertices();
	vector<TexVertex> uv = _model.GetUVCoords();
	Vertex triangles[3];

	for (int i = 0; i < _model.GetPolygonCount(); i++)
	{
		if (!polys[i].GetCull())
		{
			triangles[0] = vertices[polys[i].GetIndex(0)];
			triangles[1] = vertices[polys[i].GetIndex(1)];
			triangles[2] = vertices[polys[i].GetIndex(2)];
			triangles[0].SetUVIndex(polys[i].GetUVIndex(0));
			triangles[1].SetUVIndex(polys[i].GetUVIndex(1));
			triangles[2].SetUVIndex(polys[i].GetUVIndex(2));

			FillSolidTextured(triangles[0], triangles[1], triangles[2], bitmap);
		}
	}
}

void Rasteriser::DrawSolidTexturedCorrected(Bitmap & bitmap)
{
	// Get the polygons, vertices of the model
	vector<Polygon3D> polys = _model.GetPolygons();
	vector<Vertex> vertices = _model.GetTransformedVertices();
	vector<TexVertex> uv = _model.GetUVCoords();
	Vertex triangles[3];

	for (int i = 0; i < _model.GetPolygonCount(); i++)
	{
		if (!polys[i].GetCull())
		{
			triangles[0] = vertices[polys[i].GetIndex(0)];
			triangles[1] = vertices[polys[i].GetIndex(1)];
			triangles[2] = vertices[polys[i].GetIndex(2)];
			triangles[0].SetUVIndex(polys[i].GetUVIndex(0));
			triangles[1].SetUVIndex(polys[i].GetUVIndex(1));
			triangles[2].SetUVIndex(polys[i].GetUVIndex(2));

			int trianglesSize = sizeof(triangles) / sizeof(*triangles);
			for (int i = 0; i < trianglesSize; i++)
			{
				// calculate uoverz, voverz and zrecip for each vertex in the polygon
				float preZ = triangles[i].GetPreTransZ();
				TexVertex currentTex = uv[triangles[i].GetUVIndex()];
				float UoverZ = currentTex.GetU() / preZ;
				float VoverZ = currentTex.GetV() / preZ;
				float ZRecip = 1.0f / preZ;
				triangles[i].SetUoverZ(UoverZ);
				triangles[i].SetVoverZ(VoverZ);
				triangles[i].SetZRecip(ZRecip);
			}

			FillSolidTexturedCorrected(triangles[0], triangles[1], triangles[2], bitmap);
		}
	}
}


void Rasteriser::DrawLine(Bitmap & bitmap, float x1, float x2, float y, COLORREF col)
{
	for (int i = (int)x1; i <= (int)x2; i++)
	{
		SetPixelV(bitmap.GetDC(), i, (int)y, col);
	}
}

void Rasteriser::DrawGouraudLine(Bitmap & bitmap, float x1, float x2, float y, COLORREF startingColour, COLORREF endColour)
{
	for (int i = (int)x1; i <= (int)x2; i++)
	{
		float t = i - x1 / x2 - x1;

		int red = (int)((1 - t) * GetRValue(startingColour) + (t * GetRValue(endColour)));
		if (red > 255)
		{
			red = 255;
		}
		else if (red < 0)
		{
			red = 0;
		}
		int green = (int)((1 - t) * GetGValue(startingColour) + (t * GetGValue(endColour)));
		if (green > 255)
		{
			green = 255;
		}
		else if (green < 0)
		{
			green = 0;
		}
		int blue = (int)((1 - t) * GetBValue(startingColour) + (t * GetBValue(endColour)));
		if (blue > 255)
		{
			blue = 255;
		}
		else if (blue < 0)
		{
			blue = 0;
		}
		COLORREF col = RGB(red, green, blue);


		SetPixelV(bitmap.GetDC(), i, (int)y, col);
	}
}

void Rasteriser::FillBottomFlatTriangle(Vertex v1, Vertex v2, Vertex v3, COLORREF polyColour, Bitmap &bitmap)
{
	float slope1 = (v2.GetX() - v1.GetX()) / (v2.GetY() - v1.GetY());
	float slope2 = (v3.GetX() - v1.GetX()) / (v3.GetY() - v1.GetY());

	float curx1 = v1.GetX();
	float curx2 = v1.GetX() + 0.5f;

	for (int scanlineY = (int)v1.GetY(); scanlineY <= (int)v2.GetY(); scanlineY++)
	{
		if (curx1 < curx2)
		{
			DrawLine(bitmap, curx1, curx2, (float)scanlineY, polyColour);
		}
		else
		{
			DrawLine(bitmap, curx2, curx1, (float)scanlineY, polyColour);
		}
		curx1 += slope1;
		curx2 += slope2;
	}
}

void Rasteriser::FillTopFlatTriangle(Vertex v1, Vertex v2, Vertex v3, COLORREF polyColour, Bitmap &bitmap)
{
	float slope1 = (v3.GetX() - v1.GetX()) / (v3.GetY() - v1.GetY());
	float slope2 = (v3.GetX() - v2.GetX()) / (v3.GetY() - v2.GetY());

	float curx1 = v3.GetX();
	float curx2 = v3.GetX() + 0.5f;

	for (int scanlineY = (int)v3.GetY(); scanlineY > (int)v1.GetY(); scanlineY--)
	{
		curx1 -= slope1;
		curx2 -= slope2;
		if (curx1 < curx2)
		{
			DrawLine(bitmap, curx1, curx2, (float)scanlineY, polyColour);
		}
		else
		{
			DrawLine(bitmap, curx2, curx1, (float)scanlineY, polyColour);
		}
	}
}

float ClampValue255(float value)
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

void Rasteriser::FillBottomFlatTriangle(Vertex v1, Vertex v2, Vertex v3, Bitmap & bitmap)
{
	float slope1 = (v2.GetX() - v1.GetX()) / (v2.GetY() - v1.GetY());
	float slope2 = (v3.GetX() - v1.GetX()) / (v3.GetY() - v1.GetY());

	float x1 = v1.GetX();
	float x2 = v1.GetX() + 0.5f;

	//c1 is sole node
	COLORREF c1 = v1.GetColor();
	// c2 is bottom right
	COLORREF c2 = v2.GetColor();
	// c3 is bottom left
	COLORREF c3 = v3.GetColor();


	// Colour calculations
	float v2v1Gradient = (float)(v2.GetY() - v1.GetY());
	float colourSlopeRed1 = (float)(GetRValue(c2) - GetRValue(c1)) / v2v1Gradient;
	float colourSlopeGreen1 = (float)(GetGValue(c2) - GetGValue(c1)) / v2v1Gradient;
	float colourSlopeBlue1 = (float)(GetBValue(c2) - GetBValue(c1)) / v2v1Gradient;

	float v3v1Gradient = (float)(v3.GetY() - v1.GetY());
	float colourSlopeRed2 = (float)(GetRValue(c3) - GetRValue(c1)) / v3v1Gradient;
	float colourSlopeGreen2 = (float)(GetGValue(c3) - GetGValue(c1)) / v3v1Gradient;
	float colourSlopeBlue2 = (float)(GetBValue(c3) - GetBValue(c1)) / v3v1Gradient;

	float c1Red = GetRValue(c1);
	float c1Green = GetGValue(c1);
	float c1Blue = GetBValue(c1);
	float c2Red = c1Red;
	float c2Green = c1Green;
	float c2Blue = c1Blue;

	// Line only draws left to right so swap in case otherwise
	if (slope2 < slope1)
	{
		float slopeTmp = slope1;
		slope1 = slope2;
		slope2 = slopeTmp;

		slopeTmp = colourSlopeRed1;
		colourSlopeRed1 = colourSlopeRed2;
		colourSlopeRed2 = slopeTmp;

		slopeTmp = colourSlopeGreen1;
		colourSlopeGreen1 = colourSlopeGreen2;
		colourSlopeGreen2 = slopeTmp;

		slopeTmp = colourSlopeBlue1;
		colourSlopeBlue1 = colourSlopeBlue2;
		colourSlopeBlue2 = slopeTmp;
	}

	//COLORREF start = RGB(c1Red, c1Green, c1Blue);
	//COLORREF end = RGB(c2Red, c2Green, c2Blue);
	for (int scanlineY = (int)v1.GetY(); scanlineY <= (int)v2.GetY(); scanlineY++)
	{
		for (int xPos = (int)x1; xPos < x2; xPos++)
		{
			float t = (xPos - x1) / (x2 - x1);

			int red = (int)(((1 - t) * c1Red) + (t * c2Red));
			if (red > 255)
			{
				red = 255;
			}
			else if (red < 0)
			{
				red = 0;
			}
			int green = (int)(((1 - t) * c1Green) + (t * c2Green));
			if (green > 255)
			{
				green = 255;
			}
			else if (green < 0)
			{
				green = 0;
			}
			int blue = (int)(((1 - t) * c1Blue) + (t * c2Blue));
			if (blue > 255)
			{
				blue = 255;
			}
			else if (blue < 0)
			{
				blue = 0;
			}
			SetPixelV(bitmap.GetDC(), xPos, (int)scanlineY, RGB(red, green, blue));
		}

		x1 += slope1;
		x2 += slope2;
		
		c1Red = ClampValue255(c1Red += colourSlopeRed1);
		c1Green = ClampValue255(c1Green += colourSlopeGreen1);
		c1Blue = ClampValue255(c1Blue += colourSlopeBlue1);
		c2Red = ClampValue255(c2Red += colourSlopeRed2);
		c2Green = ClampValue255(c2Green += colourSlopeGreen2);
		c2Blue = ClampValue255(c2Blue += colourSlopeBlue2);
		//start = RGB(c1Red, c1Green, c1Blue);
		//end = RGB(c2Red, c2Green, c2Blue);
	}
}

void Rasteriser::FillTopFlatTriangle(Vertex v1, Vertex v2, Vertex v3, Bitmap & bitmap)
{
	float slope1 = (v3.GetX() - v1.GetX()) / (v3.GetY() - v1.GetY());
	float slope2 = (v3.GetX() - v2.GetX()) / (v3.GetY() - v2.GetY());

	float x1 = v3.GetX();
	float x2 = v3.GetX() + 0.5f;
	//COLORREF color = RGB(255, 255, 255);

	//if c1 != c2 issues
	COLORREF c1 = v1.GetColor();
	COLORREF c2 = v2.GetColor();
	COLORREF c3 = v3.GetColor();
	// c1 is right, c2 is left
	//c1 = RGB(255, 255, 255);
	//c2 = RGB(0, 0, 0);
	// c3 is the vertex by iteslf
	//c3 = RGB(255, 255, 255);

	// Colour calculations
	float v3v1Gradient = (float)(v3.GetY() - v1.GetY());
	float colourSlopeRed1 = (float)(GetRValue(c3) - GetRValue(c1)) / v3v1Gradient;
	float colourSlopeGreen1 = (float)(GetGValue(c3) - GetGValue(c1)) / v3v1Gradient;
	float colourSlopeBlue1 = (float)(GetBValue(c3) - GetBValue(c1)) / v3v1Gradient;

	float v3v2Gradient = (float)(v3.GetY() - v2.GetY());
	float colourSlopeRed2 = (float)(GetRValue(c3) - GetRValue(c2)) / v3v2Gradient;
	float colourSlopeGreen2 = (float)(GetGValue(c3) - GetGValue(c2)) / v3v2Gradient;
	float colourSlopeBlue2 = (float)(GetBValue(c3) - GetBValue(c2)) / v3v2Gradient;

	float c1Red = GetRValue(c3);
	float c1Green = GetGValue(c3);
	float c1Blue = GetBValue(c3);
	float c2Red = c1Red;
	float c2Green = c1Green;
	float c2Blue = c1Blue;

	if (slope1 < slope2)
	{
		float slopeTmp = slope1;
		slope1 = slope2;
		slope2 = slopeTmp;

		slopeTmp = colourSlopeRed1;
		colourSlopeRed1 = colourSlopeRed2;
		colourSlopeRed2 = slopeTmp;

		slopeTmp = colourSlopeGreen1;
		colourSlopeGreen1 = colourSlopeGreen2;
		colourSlopeGreen2 = slopeTmp;

		slopeTmp = colourSlopeBlue1;
		colourSlopeBlue1 = colourSlopeBlue2;
		colourSlopeBlue2 = slopeTmp;
	}

	//COLORREF start = RGB(0, 0, 0);
	//COLORREF end = RGB(255, 255, 255);
	for (int scanlineY = (int)v3.GetY(); scanlineY > (int)v1.GetY(); scanlineY--)
	{
		for (int xPos = (int)x1; xPos < x2; xPos++)
		{
			float t = (xPos - x1) / (x2 - x1);

			int red = (int)((1 - t) * c1Red + (t * c2Red));
			if (red > 255)
			{
				red = 255;
			}
			else if (red < 0)
			{
				red = 0;
			}
			int green = (int)((1 - t) * c1Green + (t * c2Green));
			if (green > 255)
			{
				green = 255;
			}
			else if (green < 0)
			{
				green = 0;
			}
			int blue = (int)((1 - t) * c1Blue + (t * c2Blue));
			if (blue > 255)
			{
				blue = 255;
			}
			else if (blue < 0)
			{
				blue = 0;
			}
			SetPixelV(bitmap.GetDC(), xPos, (int)scanlineY, RGB(red, green, blue));
		}
		x1 -= slope1;
		x2 -= slope2;
		c1Red = ClampValue255(c1Red -= colourSlopeRed1);
		c1Green = ClampValue255(c1Green -= colourSlopeGreen1);
		c1Blue = ClampValue255(c1Blue -= colourSlopeBlue1);
		c2Red = ClampValue255(c2Red -= colourSlopeRed2);
		c2Green = ClampValue255(c2Green -= colourSlopeGreen2);
		c2Blue = ClampValue255(c2Blue -= colourSlopeBlue2);
		//start = RGB(c1Red, c1Green, c1Blue);
		//end = RGB(c2Red, c2Green, c2Blue);
	}
}

bool CompareVertexY(Vertex &vertex, Vertex &vertex2)
{
	return vertex.GetY() < vertex2.GetY();
}

void Rasteriser::FillPolygonFlat(Vertex v1, Vertex v2, Vertex v3, COLORREF polyColour, Bitmap &bitmap)
{
	// Sort vertices so that v1 is the top vertex (lowest y value)
	vector<Vertex> vertices;
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	sort(vertices.begin(), vertices.end(), CompareVertexY);

	// Check if bottom flat or top flat or netiher
	// Comparsion removes some of the rounding errors
	float comparison = vertices[1].GetY() - vertices[2].GetY();
	float comparison2 = vertices[0].GetY() - vertices[1].GetY();
	if (comparison < 0.5f && comparison > -0.5f)
	{
		FillBottomFlatTriangle(vertices[0], vertices[1], vertices[2], polyColour, bitmap);
	}
	else if (comparison2 < 0.9f && comparison2 > -0.9f)
	{
		FillTopFlatTriangle(vertices[0], vertices[1], vertices[2], polyColour, bitmap);
	}
	else
	{
		// If neither, spilt into one of each and call appropiate methods
		float v4x = vertices[0].GetX() + ((vertices[1].GetY() - vertices[0].GetY()) / (vertices[2].GetY() - vertices[0].GetY())) * (vertices[2].GetX() - vertices[0].GetX());
		float v4y = vertices[1].GetY();
		Vertex v4(v4x, v4y, 0);
		FillBottomFlatTriangle(vertices[0], vertices[1], v4, polyColour, bitmap);
		FillTopFlatTriangle(vertices[1], v4, vertices[2], polyColour, bitmap);
	}
}

void Rasteriser::FillPolygonGouraud(Vertex v1, Vertex v2, Vertex v3, Bitmap & bitmap)
{
	// Sort vertices so that v1 is the top vertex (lowest y value)
	vector<Vertex> vertices;
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	sort(vertices.begin(), vertices.end(), CompareVertexY);

	// Check if bottom flat or top flat or netiher
	// Comparsion removes some of the rounding errors
	float comparison = vertices[1].GetY() - vertices[2].GetY();
	float comparison2 = vertices[0].GetY() - vertices[1].GetY();
	if (comparison < 0.5f && comparison > -0.5f)
	{
		FillBottomFlatTriangle(vertices[0], vertices[1], vertices[2], bitmap);
	}
	else if (comparison2 < 0.9f && comparison2 > -0.9f)
	{
		FillTopFlatTriangle(vertices[0], vertices[1], vertices[2] , bitmap);
	}
	else
	{
		// If neither, spilt into one of each and call appropiate methods
		float v4x = vertices[0].GetX() + ((vertices[1].GetY() - vertices[0].GetY()) / (vertices[2].GetY() - vertices[0].GetY())) * (vertices[2].GetX() - vertices[0].GetX());
		float v4y = vertices[1].GetY();
		Vertex v4(v4x, v4y, 0);

		COLORREF v1Col = vertices[0].GetColor();
		COLORREF v3Col = vertices[2].GetColor();
		float red = GetRValue(v1Col) + ((float)(vertices[1].GetY() - vertices[0].GetY()) / (float)(vertices[2].GetY() - vertices[0].GetY())) * (GetRValue(v3Col) - GetRValue(v1Col));
		float green = GetGValue(v1Col) + ((float)(vertices[1].GetY() - vertices[0].GetY()) / (float)(vertices[2].GetY() - vertices[0].GetY())) * (GetGValue(v3Col) - GetGValue(v1Col));
		float blue = GetBValue(v1Col) + ((float)(vertices[1].GetY() - vertices[0].GetY()) / (float)(vertices[2].GetY() - vertices[0].GetY())) * (GetBValue(v3Col) - GetBValue(v1Col));
		v4.SetColor(RGB(red, green, blue));
		
		FillBottomFlatTriangle(vertices[0], vertices[1], v4, bitmap);
		FillTopFlatTriangle(vertices[1], v4, vertices[2], bitmap);
	}
}

void Rasteriser:: FillBottomTextured(Vertex v1, Vertex v2, Vertex v3, Bitmap &bitmap)
{
	vector<TexVertex> uv = _model.GetUVCoords();
	float slope1 = (v2.GetX() - v1.GetX()) / (v2.GetY() - v1.GetY());
	float slope2 = (v3.GetX() - v1.GetX()) / (v3.GetY() - v1.GetY());

	float x1 = v1.GetX();
	float x2 = v1.GetX() + 0.5f;
	
	//c1 is sole node
	COLORREF c1 = v1.GetColor();
	// c2 is bottom left
	COLORREF c2 = v2.GetColor();
	// c3 is bottom right
	COLORREF c3 = v3.GetColor();

	//width and height are to be multiplied by u and v values respectively
	float width = (float)_model.GetTexture().GetWidth();
	float height = (float)_model.GetTexture().GetHeight();

	int uvInd0 = v1.GetUVIndex();
	float u0 = uv[uvInd0].GetU();
	float va0 = uv[uvInd0].GetV();

	int uvInd1 = v2.GetUVIndex();
	float u1 = uv[uvInd1].GetU();
	float va1 = uv[uvInd1].GetV();

	float u2 = v3.GetU();
	float va2 = v3.GetV();

	// Colour calculations
	float v2v1Gradient = (float)(v2.GetY() - v1.GetY());
	float colourSlopeRed1 = (float)(GetRValue(c2) - GetRValue(c1)) / v2v1Gradient;
	float colourSlopeGreen1 = (float)(GetGValue(c2) - GetGValue(c1)) / v2v1Gradient;
	float colourSlopeBlue1 = (float)(GetBValue(c2) - GetBValue(c1)) / v2v1Gradient;

	float v3v1Gradient = (float)(v3.GetY() - v1.GetY());
	float colourSlopeRed2 = (float)(GetRValue(c3) - GetRValue(c1)) / v3v1Gradient;
	float colourSlopeGreen2 = (float)(GetGValue(c3) - GetGValue(c1)) / v3v1Gradient;
	float colourSlopeBlue2 = (float)(GetBValue(c3) - GetBValue(c1)) / v3v1Gradient;

	float c1Red = GetRValue(c1);
	float c1Green = GetGValue(c1);
	float c1Blue = GetBValue(c1);
	float c2Red = c1Red;
	float c2Green = c1Green;
	float c2Blue = c1Blue;
	
	// U, V calculations
	float uSlope1 = ((u1 - u0) / v2v1Gradient) * width;
	float vSlope1 = ((va1 - va0) / v2v1Gradient) * height;

	float uSlope2 = ((u2 - u0) / v3v1Gradient) * width;
	float vSlope2 = ((va2 - va0) / v3v1Gradient) * height;

	float tu1 = u0 * width;
	float tv1 = va0 * height;
	float tu2 = tu1;
	float tv2 = tv1;

	// Line only draws left to right so swap in case otherwise
	if (slope2 < slope1)
	{
		float slopeTmp = slope1;
		slope1 = slope2;
		slope2 = slopeTmp;

		slopeTmp = colourSlopeRed1;
		colourSlopeRed1 = colourSlopeRed2;
		colourSlopeRed2 = slopeTmp;

		slopeTmp = colourSlopeGreen1;
		colourSlopeGreen1 = colourSlopeGreen2;
		colourSlopeGreen2 = slopeTmp;

		slopeTmp = colourSlopeBlue1;
		colourSlopeBlue1 = colourSlopeBlue2;
		colourSlopeBlue2 = slopeTmp;

		slopeTmp = uSlope1;
		uSlope1 = uSlope2;
		uSlope2 = slopeTmp;

		slopeTmp = vSlope1;
		vSlope1 = vSlope2;
		vSlope2 = slopeTmp;
	}

	for (int scanlineY = (int)v1.GetY(); scanlineY <= (int)v2.GetY(); scanlineY++)
	{
		for (int xPos = (int)x1; xPos < x2; xPos++)
		{
			float t = (xPos - x1) / (x2 - x1);

			int red = (int)(((1 - t) * c1Red) + (t * c2Red));
			if (red > 255)
			{
				red = 255;
			}
			else if (red < 0)
			{
				red = 0;
			}

			int green = (int)(((1 - t) * c1Green) + (t * c2Green));
			if (green > 255)
			{
				green = 255;
			}
			else if (green < 0)
			{
				green = 0;
			}

			int blue = (int)(((1 - t) * c1Blue) + (t * c2Blue));
			if (blue > 255)
			{
				blue = 255;
			}
			else if (blue < 0)
			{
				blue = 0;
			}
			COLORREF tcol1 = _model.GetTexture().GetTextureValue((int)tu1, (int)tv1);
			COLORREF tcol2 = _model.GetTexture().GetTextureValue((int)tu2, (int)tv2);

			int texRed = (int)(((1 - t) * GetRValue(tcol1)) + (t * GetRValue(tcol2)));
			int texGreen = (int)(((1 - t) * GetGValue(tcol1)) + (t * GetGValue(tcol2)));
			int texBlue = (int)(((1 - t) * GetBValue(tcol1)) + (t * GetBValue(tcol2)));

			float finRed = (texRed * red) / 255.0f;
			float finGreen = (texGreen * green) / 255.0f;
			float finBlue = (texBlue * blue) / 255.0f;

			SetPixelV(bitmap.GetDC(), xPos, (int)scanlineY, RGB(finRed, finGreen, finBlue));
		}

		x1 += slope1;
		x2 += slope2;

		c1Red = ClampValue255(c1Red += colourSlopeRed1);
		c1Green = ClampValue255(c1Green += colourSlopeGreen1);
		c1Blue = ClampValue255(c1Blue += colourSlopeBlue1);
		c2Red = ClampValue255(c2Red += colourSlopeRed2);
		c2Green = ClampValue255(c2Green += colourSlopeGreen2);
		c2Blue = ClampValue255(c2Blue += colourSlopeBlue2);

		tu1 += uSlope1;
		tv1 += vSlope1;
		tu2 += uSlope2;
		tv2 += vSlope2;
	}
}

void Rasteriser::FillSolidTexturedCorrected(Vertex v1, Vertex v2, Vertex v3, Bitmap & bitmap)
{
	// Sort vertices so that v1 is the top vertex (lowest y value)
	vector<Vertex> vertices;
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	sort(vertices.begin(), vertices.end(), CompareVertexY);

	// Check if bottom flat or top flat or netiher
	// Comparsion removes some of the rounding errors
	float comparison = vertices[1].GetY() - vertices[2].GetY();
	float comparison2 = vertices[0].GetY() - vertices[1].GetY();
	if (comparison < 0.5f && comparison > -0.5f)
	{
		FillBottomTexturedCorrected(vertices[0], vertices[1], vertices[2], bitmap);
	}
	else if (comparison2 < 0.9f && comparison2 > -0.9f)
	{
		FillTopTexturedCorrected(vertices[0], vertices[1], vertices[2], bitmap);
	}
	else
	{
		// If neither, spilt into one of each and call appropiate methods
		float v4x = vertices[0].GetX() + ((vertices[1].GetY() - vertices[0].GetY()) / (vertices[2].GetY() - vertices[0].GetY())) * (vertices[2].GetX() - vertices[0].GetX());
		float v4y = vertices[1].GetY();
		Vertex v4(v4x, v4y, 0);
		COLORREF v1Col = vertices[0].GetColor();
		COLORREF v3Col = vertices[2].GetColor();

		float red = GetRValue(v1Col) + ((float)(vertices[1].GetY() - vertices[0].GetY()) / (float)(vertices[2].GetY() - vertices[0].GetY())) * (GetRValue(v3Col) - GetRValue(v1Col));
		float green = GetGValue(v1Col) + ((float)(vertices[1].GetY() - vertices[0].GetY()) / (float)(vertices[2].GetY() - vertices[0].GetY())) * (GetGValue(v3Col) - GetGValue(v1Col));
		float blue = GetBValue(v1Col) + ((float)(vertices[1].GetY() - vertices[0].GetY()) / (float)(vertices[2].GetY() - vertices[0].GetY())) * (GetBValue(v3Col) - GetBValue(v1Col));
		v4.SetColor(RGB(red, green, blue));

		float vert1uz = vertices[0].GetUoverZ();
		float vert1vz = vertices[0].GetVoverZ();
		float vert1zr = vertices[0].GetZRecip();
		float vert3uz = vertices[2].GetUoverZ();
		float vert3vz = vertices[2].GetVoverZ();
		float vert3zr = vertices[2].GetZRecip();
		float newuz = vert1uz + ((float)(vertices[1].GetY() - vertices[0].GetY()) / (float)(vertices[2].GetY() - vertices[0].GetY())) * (vert3uz - vert1uz);
		float newvz = vert1vz + ((float)(vertices[1].GetY() - vertices[0].GetY()) / (float)(vertices[2].GetY() - vertices[0].GetY())) * (vert3vz - vert1vz);
		float newzr = vert1zr + ((float)(vertices[1].GetY() - vertices[0].GetY()) / (float)(vertices[2].GetY() - vertices[0].GetY())) * (vert3zr - vert1zr);
		v4.SetUoverZ(newuz);
		v4.SetVoverZ(newvz);
		v4.SetZRecip(newzr);

		FillBottomTexturedCorrected(vertices[0], vertices[1], v4, bitmap);
		FillTopTexturedCorrected(vertices[1], v4, vertices[2], bitmap);
	}
}

void Rasteriser::FillTopTexturedCorrected(Vertex v1, Vertex v2, Vertex v3, Bitmap & bitmap)
{
	float slope1 = (v3.GetX() - v1.GetX()) / (v3.GetY() - v1.GetY());
	float slope2 = (v3.GetX() - v2.GetX()) / (v3.GetY() - v2.GetY());

	float x1 = v3.GetX();
	float x2 = v3.GetX() + 0.5f;

	COLORREF c1 = v1.GetColor();
	COLORREF c2 = v2.GetColor();
	COLORREF c3 = v3.GetColor();
	// c1 is right, c2 is left
	// c3 is the vertex by iteslf

	//width and height are to be multiplied by u and v values respectively
	float width = (float)_model.GetTexture().GetWidth();
	float height = (float)_model.GetTexture().GetHeight();

	float vert1uz = v1.GetUoverZ();
	float vert1vz = v1.GetVoverZ();
	float vert1zr = v1.GetZRecip();

	float vert2uz = v2.GetUoverZ();
	float vert2vz = v2.GetVoverZ();
	float vert2zr = v2.GetZRecip();

	float vert3uz = v3.GetUoverZ();
	float vert3vz = v3.GetVoverZ();
	float vert3zr = v3.GetZRecip();

	// Colour calculations
	float v3v1Gradient = (float)(v3.GetY() - v1.GetY());
	float colourSlopeRed1 = (float)(GetRValue(c3) - GetRValue(c1)) / v3v1Gradient;
	float colourSlopeGreen1 = (float)(GetGValue(c3) - GetGValue(c1)) / v3v1Gradient;
	float colourSlopeBlue1 = (float)(GetBValue(c3) - GetBValue(c1)) / v3v1Gradient;

	float v3v2Gradient = (float)(v3.GetY() - v2.GetY());
	float colourSlopeRed2 = (float)(GetRValue(c3) - GetRValue(c2)) / v3v2Gradient;
	float colourSlopeGreen2 = (float)(GetGValue(c3) - GetGValue(c2)) / v3v2Gradient;
	float colourSlopeBlue2 = (float)(GetBValue(c3) - GetBValue(c2)) / v3v2Gradient;

	float c1Red = GetRValue(c3);
	float c1Green = GetGValue(c3);
	float c1Blue = GetBValue(c3);
	float c2Red = c1Red;
	float c2Green = c1Green;
	float c2Blue = c1Blue;

	// Tex floats with adjustment
	float uzSlope1 = ((vert3uz - vert1uz) / v3v1Gradient) * width;
	float vzSlope1 = ((vert3vz - vert1vz) / v3v1Gradient) * height;
	float zrSlope1 = ((vert3zr - vert1zr) / v3v1Gradient) * width;

	float uzSlope2 = ((vert3uz - vert2uz) / v3v2Gradient) * width;
	float vzSlope2 = ((vert3vz - vert2vz) / v3v2Gradient) * height;
	float zrSlope2 = ((vert3zr - vert2zr) / v3v2Gradient) * width;

	float uz1 = vert1uz * width;
	float vz1 = vert1vz * height;
	float zr1 = vert1zr * width;
	float uz2 = uz1;
	float vz2 = vz1;
	float zr2 = zr1;

	if (slope1 < slope2)
	{
		float slopeTmp = slope1;
		slope1 = slope2;
		slope2 = slopeTmp;

		slopeTmp = colourSlopeRed1;
		colourSlopeRed1 = colourSlopeRed2;
		colourSlopeRed2 = slopeTmp;

		slopeTmp = colourSlopeGreen1;
		colourSlopeGreen1 = colourSlopeGreen2;
		colourSlopeGreen2 = slopeTmp;

		slopeTmp = colourSlopeBlue1;
		colourSlopeBlue1 = colourSlopeBlue2;
		colourSlopeBlue2 = slopeTmp;

		slopeTmp = uzSlope1;
		uzSlope1 = uzSlope2;
		uzSlope2 = slopeTmp;

		slopeTmp = vzSlope1;
		vzSlope1 = vzSlope2;
		vzSlope2 = slopeTmp;

		slopeTmp = zrSlope1;
		zrSlope1 = zrSlope2;
		zrSlope2 = slopeTmp;
	}

	for (int scanlineY = (int)v3.GetY(); scanlineY > (int)v1.GetY(); scanlineY--)
	{
		for (int xPos = (int)x1; xPos < x2; xPos++)
		{
			float t = (xPos - x1) / (x2 - x1);

			int red = (int)((1 - t) * c1Red + (t * c2Red));
			if (red > 255)
			{
				red = 255;
			}
			else if (red < 0)
			{
				red = 0;
			}
			int green = (int)((1 - t) * c1Green + (t * c2Green));
			if (green > 255)
			{
				green = 255;
			}
			else if (green < 0)
			{
				green = 0;
			}
			int blue = (int)((1 - t) * c1Blue + (t * c2Blue));
			if (blue > 255)
			{
				blue = 255;
			}
			else if (blue < 0)
			{
				blue = 0;
			}

			float u1 = (uz1 / zr1) * width;
			float v1 = (vz1 / zr1) * height;
			float u2 = (uz2 / zr2) * width;
			float v2 = (vz2 / zr2) * height;

			COLORREF tcol1 = _model.GetTexture().GetTextureValue((int)u1, (int)v1);
			COLORREF tcol2 = _model.GetTexture().GetTextureValue((int)u2, (int)v2);

			int texRed = (int)(((1 - t) * GetRValue(tcol1)) + (t * GetRValue(tcol2)));
			int texGreen = (int)(((1 - t) * GetGValue(tcol1)) + (t * GetGValue(tcol2)));
			int texBlue = (int)(((1 - t) * GetBValue(tcol1)) + (t * GetBValue(tcol2)));


			float finRed = (texRed * red) / 255.0f;
			float finGreen = (texGreen * green) / 255.0f;
			float finBlue = (texBlue * blue) / 255.0f;

			SetPixelV(bitmap.GetDC(), xPos, (int)scanlineY, RGB(finRed, finGreen, finBlue));
		}
		x1 -= slope1;
		x2 -= slope2;
		c1Red = ClampValue255(c1Red -= colourSlopeRed1);
		c1Green = ClampValue255(c1Green -= colourSlopeGreen1);
		c1Blue = ClampValue255(c1Blue -= colourSlopeBlue1);
		c2Red = ClampValue255(c2Red -= colourSlopeRed2);
		c2Green = ClampValue255(c2Green -= colourSlopeGreen2);
		c2Blue = ClampValue255(c2Blue -= colourSlopeBlue2);

		uz1 -= uzSlope1;
		vz1 -= vzSlope1;
		zr1 -= zrSlope1;
		uz2 -= uzSlope2;
		vz2 -= vzSlope2;
		zr2 -= zrSlope2;
	}
}

void Rasteriser::FillBottomTexturedCorrected(Vertex v1, Vertex v2, Vertex v3, Bitmap & bitmap)
{
	vector<TexVertex> uv = _model.GetUVCoords();
	float slope1 = (v2.GetX() - v1.GetX()) / (v2.GetY() - v1.GetY());
	float slope2 = (v3.GetX() - v1.GetX()) / (v3.GetY() - v1.GetY());

	float x1 = v1.GetX();
	float x2 = v1.GetX() + 0.5f;

	//c1 is sole node
	COLORREF c1 = v1.GetColor();
	// c2 is bottom left
	COLORREF c2 = v2.GetColor();
	// c3 is bottom right
	COLORREF c3 = v3.GetColor();

	//width and height are to be multiplied by u and v values respectively
	float width = (float)_model.GetTexture().GetWidth();
	float height = (float)_model.GetTexture().GetHeight();

	float vert1uz = v1.GetUoverZ();
	float vert1vz = v1.GetVoverZ();
	float vert1zr = v1.GetZRecip();

	float vert2uz = v2.GetUoverZ();
	float vert2vz = v2.GetVoverZ();
	float vert2zr = v2.GetZRecip();

	float vert3uz = v3.GetUoverZ();
	float vert3vz = v3.GetVoverZ();
	float vert3zr = v3.GetZRecip();

	// Colour calculations
	float v2v1Gradient = (float)(v2.GetY() - v1.GetY());
	float colourSlopeRed1 = (float)(GetRValue(c2) - GetRValue(c1)) / v2v1Gradient;
	float colourSlopeGreen1 = (float)(GetGValue(c2) - GetGValue(c1)) / v2v1Gradient;
	float colourSlopeBlue1 = (float)(GetBValue(c2) - GetBValue(c1)) / v2v1Gradient;

	float v3v1Gradient = (float)(v3.GetY() - v1.GetY());
	float colourSlopeRed2 = (float)(GetRValue(c3) - GetRValue(c1)) / v3v1Gradient;
	float colourSlopeGreen2 = (float)(GetGValue(c3) - GetGValue(c1)) / v3v1Gradient;
	float colourSlopeBlue2 = (float)(GetBValue(c3) - GetBValue(c1)) / v3v1Gradient;

	float c1Red = GetRValue(c1);
	float c1Green = GetGValue(c1);
	float c1Blue = GetBValue(c1);
	float c2Red = c1Red;
	float c2Green = c1Green;
	float c2Blue = c1Blue;

	// Tex floats with adjustment
	float uzSlope1 = ((vert2uz - vert1uz) / v2v1Gradient) * width;
	float vzSlope1 = ((vert2vz - vert1vz) / v2v1Gradient) * height;
	float zrSlope1 = ((vert2zr - vert1zr) / v2v1Gradient) * width;

	float uzSlope2 = ((vert3uz - vert1uz) / v3v1Gradient) * width;
	float vzSlope2 = ((vert3vz - vert1vz) / v3v1Gradient) * height;
	float zrSlope2 = ((vert3zr - vert1zr) / v3v1Gradient) * width;

	float uz1 = vert1uz * width;
	float vz1 = vert1vz * height;
	float zr1 = vert1zr * width;
	float uz2 = uz1;
	float vz2 = vz1;
	float zr2 = zr1;

	// Line only draws left to right so swap in case otherwise
	if (slope2 < slope1)
	{
		float slopeTmp = slope1;
		slope1 = slope2;
		slope2 = slopeTmp;

		slopeTmp = colourSlopeRed1;
		colourSlopeRed1 = colourSlopeRed2;
		colourSlopeRed2 = slopeTmp;

		slopeTmp = colourSlopeGreen1;
		colourSlopeGreen1 = colourSlopeGreen2;
		colourSlopeGreen2 = slopeTmp;

		slopeTmp = colourSlopeBlue1;
		colourSlopeBlue1 = colourSlopeBlue2;
		colourSlopeBlue2 = slopeTmp;

		slopeTmp = uzSlope1;
		uzSlope1 = uzSlope2;
		uzSlope2 = slopeTmp;

		slopeTmp = vzSlope1;
		vzSlope1 = vzSlope2;
		vzSlope2 = slopeTmp;

		slopeTmp = zrSlope1;
		zrSlope1 = zrSlope2;
		zrSlope2 = slopeTmp;
	}

	for (int scanlineY = (int)v1.GetY(); scanlineY <= (int)v2.GetY(); scanlineY++)
	{
		for (int xPos = (int)x1; xPos < x2; xPos++)
		{
			float t = (xPos - x1) / (x2 - x1);

			int red = (int)(((1 - t) * c1Red) + (t * c2Red));
			if (red > 255)
			{
				red = 255;
			}
			else if (red < 0)
			{
				red = 0;
			}

			int green = (int)(((1 - t) * c1Green) + (t * c2Green));
			if (green > 255)
			{
				green = 255;
			}
			else if (green < 0)
			{
				green = 0;
			}

			int blue = (int)(((1 - t) * c1Blue) + (t * c2Blue));
			if (blue > 255)
			{
				blue = 255;
			}
			else if (blue < 0)
			{
				blue = 0;
			}

			float u1 = (uz1 / zr1) * width;
			float v1 = (vz1 / zr1) * height;
			float u2 = (uz2 / zr2) * width;
			float v2 = (vz2 / zr2) * height;

			COLORREF tcol1 = _model.GetTexture().GetTextureValue((int)u1, (int)v1);
			COLORREF tcol2 = _model.GetTexture().GetTextureValue((int)u2, (int)v2);

			int texRed = (int)(((1 - t) * GetRValue(tcol1)) + (t * GetRValue(tcol2)));
			int texGreen = (int)(((1 - t) * GetGValue(tcol1)) + (t * GetGValue(tcol2)));
			int texBlue = (int)(((1 - t) * GetBValue(tcol1)) + (t * GetBValue(tcol2)));


			float finRed = (texRed * red) / 255.0f;
			float finGreen = (texGreen * green) / 255.0f;
			float finBlue = (texBlue * blue) / 255.0f;

			SetPixelV(bitmap.GetDC(), xPos, (int)scanlineY, RGB(finRed, finGreen, finBlue));
		}

		x1 += slope1;
		x2 += slope2;

		c1Red = ClampValue255(c1Red += colourSlopeRed1);
		c1Green = ClampValue255(c1Green += colourSlopeGreen1);
		c1Blue = ClampValue255(c1Blue += colourSlopeBlue1);
		c2Red = ClampValue255(c2Red += colourSlopeRed2);
		c2Green = ClampValue255(c2Green += colourSlopeGreen2);
		c2Blue = ClampValue255(c2Blue += colourSlopeBlue2);

		uz1 += uzSlope1;
		vz1 += vzSlope1;
		zr1 += zrSlope1;
		uz2 += uzSlope2;
		vz2 += vzSlope2;
		zr2 += zrSlope2;
	}
}

void Rasteriser:: FillTopTextured(Vertex v1, Vertex v2, Vertex v3, Bitmap &bitmap)
{
	float slope1 = (v3.GetX() - v1.GetX()) / (v3.GetY() - v1.GetY());
	float slope2 = (v3.GetX() - v2.GetX()) / (v3.GetY() - v2.GetY());

	float x1 = v3.GetX();
	float x2 = v3.GetX() + 0.5f;

	COLORREF c1 = v1.GetColor();
	COLORREF c2 = v2.GetColor();
	COLORREF c3 = v3.GetColor();
	// c1 is right, c2 is left
	// c3 is the vertex by iteslf

	//width and height are to be multiplied by u and v values respectively
	float width = (float)_model.GetTexture().GetWidth();
	float height = (float)_model.GetTexture().GetHeight();

	vector<TexVertex> uv = _model.GetUVCoords();

	int uvInd0 = v1.GetUVIndex();
	float u0 = uv[uvInd0].GetU();
	float va0 = uv[uvInd0].GetV();

	float u1 = v2.GetU();
	float va1 = v2.GetV();

	int uvInd2 = v3.GetUVIndex();
	float u2 = uv[uvInd2].GetU();
	float va2 = uv[uvInd2].GetV();

	// Colour calculations
	float v3v1Gradient = (float)(v3.GetY() - v1.GetY());
	float colourSlopeRed1 = (float)(GetRValue(c3) - GetRValue(c1)) / v3v1Gradient;
	float colourSlopeGreen1 = (float)(GetGValue(c3) - GetGValue(c1)) / v3v1Gradient;
	float colourSlopeBlue1 = (float)(GetBValue(c3) - GetBValue(c1)) / v3v1Gradient;

	float v3v2Gradient = (float)(v3.GetY() - v2.GetY());
	float colourSlopeRed2 = (float)(GetRValue(c3) - GetRValue(c2)) / v3v2Gradient;
	float colourSlopeGreen2 = (float)(GetGValue(c3) - GetGValue(c2)) / v3v2Gradient;
	float colourSlopeBlue2 = (float)(GetBValue(c3) - GetBValue(c2)) / v3v2Gradient;

	float c1Red = GetRValue(c3);
	float c1Green = GetGValue(c3);
	float c1Blue = GetBValue(c3);
	float c2Red = c1Red;
	float c2Green = c1Green;
	float c2Blue = c1Blue;

	//Tex floats w/o perspective adjustment
	float uSlope1 = ((u2 - u0) / v3v1Gradient) * width;
	float vSlope1 = ((va2 - va0) / v3v1Gradient) * height;

	float uSlope2 = ((u2 - u1) / v3v2Gradient) * width;
	float vSlope2 = ((va2 - va1) / v3v2Gradient) * height;

	float tu1 = u2 * width;
	float tv1 = va2 * height;
	float tu2 = tu1;
	float tv2 = tv1;

	if (slope1 < slope2)
	{
		float slopeTmp = slope1;
		slope1 = slope2;
		slope2 = slopeTmp;

		slopeTmp = colourSlopeRed1;
		colourSlopeRed1 = colourSlopeRed2;
		colourSlopeRed2 = slopeTmp;

		slopeTmp = colourSlopeGreen1;
		colourSlopeGreen1 = colourSlopeGreen2;
		colourSlopeGreen2 = slopeTmp;

		slopeTmp = colourSlopeBlue1;
		colourSlopeBlue1 = colourSlopeBlue2;
		colourSlopeBlue2 = slopeTmp;

		slopeTmp = uSlope1;
		uSlope1 = uSlope2;
		uSlope2 = slopeTmp;

		slopeTmp = vSlope1;
		vSlope1 = vSlope2;
		vSlope2 = slopeTmp;
	}

	for (int scanlineY = (int)v3.GetY(); scanlineY > (int)v1.GetY(); scanlineY--)
	{
		for (int xPos = (int)x1; xPos < x2; xPos++)
		{
			float t = (xPos - x1) / (x2 - x1);

			int red = (int)((1 - t) * c1Red + (t * c2Red));
			if (red > 255)
			{
				red = 255;
			}
			else if (red < 0)
			{
				red = 0;
			}
			int green = (int)((1 - t) * c1Green + (t * c2Green));
			if (green > 255)
			{
				green = 255;
			}
			else if (green < 0)
			{
				green = 0;
			}
			int blue = (int)((1 - t) * c1Blue + (t * c2Blue));
			if (blue > 255)
			{
				blue = 255;
			}
			else if (blue < 0)
			{
				blue = 0;
			}

			COLORREF tcol1 = _model.GetTexture().GetTextureValue((int)tu1, (int)tv1);
			COLORREF tcol2 = _model.GetTexture().GetTextureValue((int)tu2, (int)tv2);

			int texRed = (int)(((1 - t) * GetRValue(tcol1)) + (t * GetRValue(tcol2)));
			int texGreen = (int)(((1 - t) * GetGValue(tcol1)) + (t * GetGValue(tcol2)));
			int texBlue = (int)(((1 - t) * GetBValue(tcol1)) + (t * GetBValue(tcol2))); 

			float finRed = (texRed * red) / 255.0f;
			float finGreen = (texGreen * green) / 255.0f;
			float finBlue = (texBlue * blue) / 255.0f;

			SetPixelV(bitmap.GetDC(), xPos, (int)scanlineY, RGB(finRed, finGreen, finBlue));
		}
		x1 -= slope1;
		x2 -= slope2;
		c1Red = ClampValue255(c1Red -= colourSlopeRed1);
		c1Green = ClampValue255(c1Green -= colourSlopeGreen1);
		c1Blue = ClampValue255(c1Blue -= colourSlopeBlue1);
		c2Red = ClampValue255(c2Red -= colourSlopeRed2);
		c2Green = ClampValue255(c2Green -= colourSlopeGreen2);
		c2Blue = ClampValue255(c2Blue -= colourSlopeBlue2);

		tu1 -= uSlope1;
		tv1 -= vSlope1;
		tu2 -= uSlope2;
		tv2 -= vSlope2;
	}
}

void Rasteriser::FillSolidTextured(Vertex v1, Vertex v2, Vertex v3, Bitmap & bitmap)
{
	// Sort vertices so that v1 is the top vertex (lowest y value)
	vector<Vertex> vertices;
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	sort(vertices.begin(), vertices.end(), CompareVertexY);

	// Check if bottom flat or top flat or netiher
	// Comparsion removes some of the rounding errors
	float comparison = vertices[1].GetY() - vertices[2].GetY();
	float comparison2 = vertices[0].GetY() - vertices[1].GetY();
	if (comparison < 0.5f && comparison > -0.5f)
	{
		FillBottomTextured(vertices[0], vertices[1], vertices[2], bitmap);
	}
	if (comparison2 < 0.9f && comparison2 > -0.9f)
	{
		FillTopTextured(vertices[0], vertices[1], vertices[2], bitmap);
	}
	else
	{
		// If neither, spilt into one of each and call appropiate methods
		float v4x = vertices[0].GetX() + ((vertices[1].GetY() - vertices[0].GetY()) / (vertices[2].GetY() - vertices[0].GetY())) * (vertices[2].GetX() - vertices[0].GetX());
		float v4y = vertices[1].GetY();
		Vertex v4(v4x, v4y, 0);
		COLORREF v1Col = vertices[0].GetColor();
		COLORREF v3Col = vertices[2].GetColor();

		float red = GetRValue(v1Col) + ((float)(vertices[1].GetY() - vertices[0].GetY()) / (float)(vertices[2].GetY() - vertices[0].GetY())) * (GetRValue(v3Col) - GetRValue(v1Col));
		float green = GetGValue(v1Col) + ((float)(vertices[1].GetY() - vertices[0].GetY()) / (float)(vertices[2].GetY() - vertices[0].GetY())) * (GetGValue(v3Col) - GetGValue(v1Col));
		float blue = GetBValue(v1Col) + ((float)(vertices[1].GetY() - vertices[0].GetY()) / (float)(vertices[2].GetY() - vertices[0].GetY())) * (GetBValue(v3Col) - GetBValue(v1Col));
		v4.SetColor(RGB(red, green, blue));

		// code pre- perspective correction
		vector<TexVertex> uv = _model.GetUVCoords();
		float u1 = uv[vertices[0].GetUVIndex()].GetU();
		float va1 = uv[vertices[0].GetUVIndex()].GetV();
		float u3 = uv[vertices[2].GetUVIndex()].GetU();
		float va3 = uv[vertices[2].GetUVIndex()].GetV();
		float newU = u1 + ((float)(vertices[1].GetY() - vertices[0].GetY()) / (float)(vertices[2].GetY() - vertices[0].GetY())) * (u3 - u1);
		float newV = va1 + ((float)(vertices[1].GetY() - vertices[0].GetY()) / (float)(vertices[2].GetY() - vertices[0].GetY())) * (va3 - va1);
		v4.SetU(newU);
		v4.SetV(newV); 

		float vert1uz = vertices[0].GetUoverZ();
		float vert1vz = vertices[0].GetVoverZ();
		float vert1zr = vertices[0].GetZRecip();
		float vert3uz = vertices[2].GetUoverZ();
		float vert3vz = vertices[2].GetVoverZ();
		float vert3zr = vertices[2].GetZRecip();
		float newuz = vert1uz + ((float)(vertices[1].GetY() - vertices[0].GetY()) / (float)(vertices[2].GetY() - vertices[0].GetY())) * (vert3uz - vert1uz);
		float newvz = vert1vz + ((float)(vertices[1].GetY() - vertices[0].GetY()) / (float)(vertices[2].GetY() - vertices[0].GetY())) * (vert3vz - vert1vz);
		float newzr = vert1zr + ((float)(vertices[1].GetY() - vertices[0].GetY()) / (float)(vertices[2].GetY() - vertices[0].GetY())) * (vert3zr - vert1zr);
		v4.SetUoverZ(newuz);
		v4.SetVoverZ(newvz);
		v4.SetZRecip(newzr);

		FillBottomTextured(vertices[0], vertices[1], v4, bitmap);
		FillTopTextured(vertices[1], v4, vertices[2], bitmap);
	}
}

//able to pass in any matrix and set a matrix of which to transform the model
void Rasteriser::SetTransformation(Matrix &matrix)
{
	_transformation = matrix;
}

// Output a string to the bitmap at co-ordinates 10, 10
// 
// Parameters: bitmap - A reference to the bitmap object
//             text   - A pointer to a string of wide characters
//
// For example, you might call this using:
//
//   DrawString(bitmap, L"Text to display");

void Rasteriser::DrawString(Bitmap &bitmap, LPCTSTR text)
{
	HDC hdc = bitmap.GetDC();
	HFONT hFont, hOldFont;

	// Retrieve a handle to the variable stock font.  
	hFont = hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Myfont"));

	// Select the variable stock font into the specified device context. 
	if (hOldFont = (HFONT)SelectObject(hdc, hFont))
	{
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkColor(hdc, RGB(0, 0, 0));

		// Display the text string.  
		TextOut(hdc, 10, 10, text, lstrlen(text));

		// Restore the original font.        
		SelectObject(hdc, hOldFont);
	}
	DeleteObject(hFont);
}