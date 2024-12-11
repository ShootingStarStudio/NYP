/**
 BackgroundEntity
 @brief A class which stores a background entity for the game states
 By: Lim Chian Song
 Date: May 2020
 */
#pragma once

// Include Singleton template
#include "DesignPatterns\SingletonTemplate.h"

// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include CEntity2D
#include "Primitives/Entity2D.h"

//CS: Include Mesh.h to use to draw (include vertex and index buffers)
#include "Primitives/Mesh.h"

class CBackgroundEntity : public CEntity2D
{
public:
	// Constructor
	CBackgroundEntity(std::string name);
	// Destructor
	~CBackgroundEntity();

	// Init
	bool Init(void);

	// DeInitialise this instance
	virtual void DeInit(void);

	// Render this class instance
	void Render(void);

protected:
	std::string sTextureName;
};