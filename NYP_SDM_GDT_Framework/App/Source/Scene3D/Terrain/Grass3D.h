/**
 CGrass3D
 By: Toh Da Jun
 Date: Sept 2023
 */
#pragma once

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include "vertexBufferObject.h"
#include "texture.h"

// Include CTerrain
#include "../Terrain/TerrainManager.h"

#include <string>
using namespace std;

// A class which renders a grass entity using billboards
class CGrass3D : public CEntity3D
{
public:
	// Default Constructor
	CGrass3D(void);

	// Constructor with vectors
	CGrass3D(const glm::vec3 vec3Position);

	// Destructor
	virtual ~CGrass3D(void);

	// Initialise this class instance
	bool Init(void);

	// DeInitialise this instance
	virtual void DeInit(void);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	// Get methods
	bool GetWindStatus(void) const;
	glm::vec2 GetMinPos(void) const; // Get the vMinPos
	glm::vec2 GetMaxPos(void) const; // Get the vMaxPos
	float GetGrassMinSize(void) const; // Get the vGrassMinSize
	float GetSpacing(void) const; // Get the fSpacing

	// Set methods
	void SetWindStatus(const bool bWindStatus);
	void SetMinMaxPos(glm::vec2 vMinPos, glm::vec2 vMaxPos);
	void SetGrassMinSize(const float vGrassMinSize);
	void SetSpacing(const float fSpacing);

	// Print Self
	virtual void PrintSelf(void);

protected:
	// OpenGL variables
	unsigned int VAO, VBO;
	// Texture IDs
	unsigned int iTextureID1, iTextureID2;
	// The vector of positions where the grasses will spawn
	std::vector<glm::vec3> positions;
	// The time passed since this class started running
	float fTimePassed;

	// The size of the grass region
	glm::vec2 vMinPos, vMaxPos;
	// The minimum size of a grass
	float fGrassMinSize;
	// The spacing between each grass
	float fSpacing;

	// The distance to offset from the terrain
	const float fOffsetY = 0.0f;

	// Variables for the wind
	bool bWindStatus;
	glm::vec2 vec2WindStrength;
	float fWindStrength;

	// The handle to the CTerrainManager class instance
	CTerrainManager* pTerrainManager;
};
