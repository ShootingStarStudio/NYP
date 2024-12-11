/**
 CPlayer2D
 @brief A class representing the player object
 By: Toh Da Jun
 Date: Mar 2020
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

// Include the Map2D as we will use it to check the player's movements and actions
class CMap2D;

// Include Keyboard controller
#include "Inputs\KeyboardController.h"

// Include Sound Controller
#include "..\SoundController\SoundController.h"

// Include Physics2D
#include "Physics2D.h"

// Include AnimatedSprites
#include "Primitives/SpriteAnimation.h"

// Include InventoryManager
#include "InventoryManager.h"

#include "GameManager.h"

#include "Projectile2D.h"

class CPlayer2D : public CSingletonTemplate<CPlayer2D>, public CEntity2D
{
	friend CSingletonTemplate<CPlayer2D>;
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
	//global interaction damage
	void takeDamage(int damage);

	bool getInteractFrame(void) {
		return interactframe;
	}
	void setInteractFrame(bool interact) {
		interactframe = interact;
	}

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

	// Keyboard Controller singleton instance
	CKeyboardController* cKeyboardController;

	// sound controller singleton instance
	CSoundController* cSoundController;

	// Physics
	CPhysics2D cPhysics2D;

	//CS: Animated Sprite
	CSpriteAnimation* AnimSprite;

	// Current color
	glm::vec4 runtimeColour;

	// InventoryManager
	CInventoryManager* cInventoryManager;
	// InventoryItem
	CInventoryItem* cInventoryItem;

	CGameManager* cGameManager;

	CProjectile2D* cProjectile2D;

	// Count the number of jumps
	int iJumpCount;

	// Constructor
	CPlayer2D(void);

	// Destructor
	virtual ~CPlayer2D(void);

	// Constraint the player's position within a boundary
	void Constraint(DIRECTION eDirection = LEFT);

	// Check if a position is possible to move into
	bool CheckPosition(DIRECTION eDirection);

	// Check if the player is in mid-air
	bool IsMidAir(void);

	// Update Jump or Fall
	void UpdateJumpFall(const double dElapsedTime = 0.0166666666666667);

	// Let player interact with the map
	void InteractWithMap(void);

	float multiplier_speed;
	float multiplier_jump;
	bool climbable;

	bool ranged = false;		//controller for player state
	char direction = 'D';

	double deathElapsed = 0;
	double respawnTimer = 3;

	double AnimWait = 0.f;

	bool interactframe = false;
};

