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

// Include the Map2D as we will use it to check the player's movements and actions
class CMap2D;

// Include Keyboard controller
#include "Inputs\KeyboardController.h"

// Include Physics2D
#include "Physics2D.h"

// Include AnimatedSprites
#include "Primitives/SpriteAnimation.h"

// Include InventoryManager
#include "InventoryManager.h"

#include "GameManager.h"

// include sound controller
#include "..\SoundController\SoundController.h"

class CProjectile2D : public CSingletonTemplate<CProjectile2D>, public CEntity2D
{
	friend CSingletonTemplate<CProjectile2D>;
public:

	// Init
	bool Init(void);

	// Reset
	bool Reset(void);

	// Update
	void Update(const double dElapsedTime);

	// PreRender
	void PreRender(void);

	// Render
	void Render(void);

	// PostRender
	void PostRender(void);

	bool isLeft;
	bool projectile = false;
	void SpawnProjectile(bool DirectionL, glm::vec2 position);
	float speed = 4.f;

	unsigned int Left, Right;

protected:
	enum DIRECTION
	{
		LEFT = 0,
		RIGHT = 1,
		UP = 2,
		DOWN = 3,
		NUM_DIRECTIONS
	};

	glm::vec2 vec2OldIndex;

	// Handler to the CMap2D instance
	CMap2D* cMap2D;

	//CS: Animated Sprite
	CSpriteAnimation* animatedSprites;

	// Current color
	glm::vec4 runtimeColour;

	CGameManager* cGameManager;

	//handler to CSoundController
	CSoundController* cSoundController;

	// Constructor
	CProjectile2D(void);

	// Destructor
	virtual ~CProjectile2D(void);

	// Constraint the player's position within a boundary
	void Constraint(DIRECTION eDirection = LEFT);

	// Check if a position is possible to move into
	bool CheckPosition(DIRECTION eDirection);

	// Let player interact with the map
	void InteractWithMap(void);
};

