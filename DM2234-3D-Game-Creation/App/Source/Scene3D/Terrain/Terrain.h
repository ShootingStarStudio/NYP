/**
CTerrain
@brief Wraps FreeType heightmap loading and rendering, also allowing
	   to use multiple layers of textures with transitions between them.
Adapted from: https://www.mbsoftworks.sk/tutorials/opengl3/21-multilayered-terrain/
Adapted By: Toh Da Jun
Date: Sep 2021
*/
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include Entity3D
#include <Primitives/Entity3D.h>

#include "vertexBufferObject.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include <string>

#include "ppm.h" 
#include "PerlinNoise.h" 

using namespace std;

struct terrain_data {
	//string sImagePath;
	unsigned int uiVAO;

	// The VBOs of the heightmap
	CVertexBufferObject vboHeightmapData;
	// The indices for the VBOs of the heightmap
	CVertexBufferObject vboHeightmapIndices;
	// The raw vertex data
	vector< vector< glm::vec3> > vVertexData;
	// The raw coordinates data
	vector< vector< glm::vec2> > vCoordsData;
};

class CTerrain : public CSingletonTemplate<CTerrain>, public CEntity3D
{
	friend CSingletonTemplate<CTerrain>;
public:
	// Initialise this class instance
	bool Init(void);
	bool Init(const string sImagePath);

	// Set model
	virtual void SetModel(const glm::mat4 model);
	// Set view
	virtual void SetView(const glm::mat4 view);
	// Set projection
	virtual void SetProjection(const glm::mat4 projection);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	bool LoadHeightMapFromImage(const string sImagePath);
	void ReleaseHeightmap(terrain_data terrain);

	void SetRenderSize(const float fQuadSize, const float fHeight);
	void SetRenderSize(const float fRenderX, const float fHeight, const float fRenderZ);

	// Get Height Map rows
	int GetNumHeightmapRows(void) const;
	// Get Height Map columns
	int GetNumHeightmapCols(void) const;
	// Get the height at a X- and Z-coordinate
	float GetHeight(const float fX, const float fZ);
	// Get the vMinPos
	glm::vec3 GetMinPos(void) const;
	// Get the vMaxPos
	glm::vec3 GetMaxPos(void) const;

	// Print Self
	void PrintSelf(void);

	virtual bool Generate();				//generate new map and store it in the next index

	void Increment();						//go to the next map index

protected:
	// Constructor
	CTerrain(void);
	// Destructor
	~CTerrain(void);

	int iRows;
	int iCols;

	// The Render Scale for this terrain
	glm::vec3 vRenderScale;
	// The min and max positions for this terrain
	glm::vec3 vMinPos, vMaxPos;

	//current index
	int terrain_Index = 0;		//iterator for terrain vector
	std::vector<terrain_data> terrain_vector;

	unsigned int width = 250, height = 250;
	unsigned int octave = 4;						//the number of iterations to get the fractal noise, high values are a waste of computation time since the images will be nearly black afterwards
	double lacurinity = 2.0;						//frequncy, the magnification of the number of grids when octaves increment, thus having more detail than the previous noise
	double persistanace = 0.5;						//amplitude, how much the octave influences the noise map, higher persistance will create regular noise

	float sealevel = 12.5f;
};