/**
 CEnemy3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include CSolidObject
#include "SolidObject.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include CTerrain
#include "../Terrain/Terrain.h"

#include <string>
using namespace std;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class CCollectible3D : public CSolidObject
{
public:

	// Default Constructor
	CCollectible3D(void);

	// Constructor with vectors
	CCollectible3D(	const glm::vec3 vec3Position);

	// Destructor
	virtual ~CCollectible3D(void);

	// Initialise this class instance
	bool Init(void);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	virtual int getID(void);
	virtual float getValue(void);

protected:

	// Constraint the Enemy's position
	void Constraint(void);

	int ID;
	float value;
};
