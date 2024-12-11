/**
 CEnemy3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

#include "Collectibles3D.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include CTerrain
#include "../Terrain/Terrain.h"

#include <string>
using namespace std;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class CCollectible3D_Health : public CCollectible3D
{
public:

	// Default Constructor
	CCollectible3D_Health(void);

	// Constructor with vectors
	CCollectible3D_Health(	const glm::vec3 vec3Position);

	// Destructor
	virtual ~CCollectible3D_Health(void);

	// Initialise this class instance
	bool Init(void);
};
