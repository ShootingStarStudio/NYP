/**
 CHut_Concrete: This class renders a Concrete hut model with textures
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

// Include CSolidObject
#include "SolidObject.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include Camera
#include "../Camera.h"

#include <string>
using namespace std;

class CHut_Concrete : public CSolidObject
{
public:
	// Default Constructor
	CHut_Concrete(void);

	// Constructor with vectors
	CHut_Concrete(	const glm::vec3 vec3Position,
					const glm::vec3 vec3Front = glm::vec3(0.0f, 0.0f, -1.0f));

	// Destructor
	virtual ~CHut_Concrete(void);

	// Initialise this class instance
	bool Init(void);

	// Load a model and texture
	bool LoadModelAndTexture(	const char* filenameModel, 
								const char* filenameTexture, 
								GLuint& iTextureID);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	// Print Self
	virtual void PrintSelf(void);
};
