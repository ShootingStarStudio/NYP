/**
 CEnemy3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Enemy3D.h"

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

// Include ImageLoader
#include "System\ImageLoader.h"

#include <iostream>
using namespace std;

// Set this to true if printing the debug info
#define _DEBUG_FSM false

/**
 @brief Default Constructor
 */
CEnemy3D::CEnemy3D(void)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(-90.0f)
	, fPitch(0.0f)
	, fRotationSensitivity(0.1f)
	, pCamera(NULL)
	, pPrimaryWeapon(NULL)
	, pSecondaryWeapon(NULL)
	, iCurrentWeapon(0)
	, pPlayer3D(NULL)
	, pTerrainManager(NULL)
	, pWaypointManager(NULL)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, +fHeightOffset, 0.0f);

	// Update the vectors for this enemy
	UpdateEnemyVectors();
}

/**
 @brief Constructor with vectors
 @param vec3Position A const glm::vec3 variable which contains the position of the camera
 @param vec3Front A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CEnemy3D::CEnemy3D(	const glm::vec3 vec3Position,
					const glm::vec3 vec3Front,
					const float fYaw,
					const float fPitch)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(fYaw)
	, fPitch(fPitch)
	, fRotationSensitivity(0.1f)
	, pCamera(NULL)
	, pPrimaryWeapon(NULL)
	, pSecondaryWeapon(NULL)
	, iCurrentWeapon(0)
	, pPlayer3D(NULL)
	, pTerrainManager(NULL)
{
	this->vec3Position = vec3Position;
	this->vec3Position.y += +fHeightOffset;
	this->vec3Front = vec3Front;
	this->fYaw = fYaw;
	this->fPitch = fPitch;

	// Update the vectors for this enemy
	UpdateEnemyVectors();
}

/**
 @brief Destructor
 */
CEnemy3D::~CEnemy3D(void)
{
	if (pWaypointManager)
	{
		// We set it to NULL only since it was declared somewhere else
		pWaypointManager = NULL;
	}

	if (pTerrainManager)
	{
		// We set it to NULL only since it was declared somewhere else
		pTerrainManager = NULL;
	}

	if (pPlayer3D)
	{
		// We set it to NULL only since it was declared somewhere else
		pPlayer3D = NULL;
	}

	if (pCamera)
	{
		// We set it to NULL only since it was declared somewhere else
		pCamera = NULL;
	}

	// Delete the rendering objects in the graphics card
	glDeleteVertexArrays(1, &VAO);
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CEnemy3D::Init(void)
{
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::NPC);

	// Initialise the pPlayer3D
	pPlayer3D = CPlayer3D::GetInstance();

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	p3DMesh = CMeshBuilder::GenerateBox(glm::vec4(1, 1, 1, 1));

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene3D_Enemy_01.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Enemy_01.tga" << endl;
		return false;
	}

	// Store the handler to the TerrainManager
	pTerrainManager = CTerrainManager::GetInstance();

	// Movement Control
	fMovementSpeed = 1.5f;
	iCurrentNumMovement = 0;
	iMaxNumMovement = 100;

	// Detection distance for player
	fDetectionDistance = 10.0f;

	// Init pWaypointManager
	pWaypointManager = new CWaypointManager;
	pWaypointManager->Init();

	// Add in some test Waypoints
	float fCheckHeight = pTerrainManager->GetHeight(0.0f, -30.0f);
	int m_iWayPointID = pWaypointManager->AddWaypoint(glm::vec3(0.0f, fCheckHeight, -30.0f));
	fCheckHeight = pTerrainManager->GetHeight(20.0f, -20.0f);
	m_iWayPointID = pWaypointManager->AddWaypoint(m_iWayPointID, glm::vec3(30.0f, fCheckHeight, 0.0f));
	fCheckHeight = pTerrainManager->GetHeight(-20.0f, -30.0f);
	m_iWayPointID = pWaypointManager->AddWaypoint(m_iWayPointID, glm::vec3(-30.0f, fCheckHeight, 0.0f));

	pWaypointManager->PrintSelf();

	// Let the NPC face the nearest waypoint
	vec3Front = glm::normalize((pWaypointManager->GetNearestWaypoint(vec3Position)->GetPosition() - vec3Position));
	UpdateFrontAndYaw();

	return true;
}

/**
 @brief Attach a camera to this class instance
 @param pCamera A CCamera* variable which contains the camera
 */
void CEnemy3D::AttachCamera(CCamera* pCamera)
{
	// Set the camera to the player
	this->pCamera = pCamera;

	// Update the camera's attributes with the player's attributes
	if (pCamera)
	{
		pCamera->vec3Position = vec3Position;
		pCamera->vec3Front = vec3Front;
	}
}

/**
 @brief Check if a camera ia attached to this class instance
 @return true if a camera is attached, else false
 */
bool CEnemy3D::IsCameraAttached(void)
{
	if (pCamera)
		return true;
	return false;
}

/**
 @brief Set Weapon to this class instance
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 @param cWeaponInfo A CWeaponInfo* variable which contains the weapon info
 */
void CEnemy3D::SetWeapon(const int iSlot, CWeaponInfo* cWeaponInfo)
{
	if (iSlot == 0)
		pPrimaryWeapon = cWeaponInfo;
	else if (iSlot == 1)
		pSecondaryWeapon = cWeaponInfo;
}

/**
 @brief Get Weapon of this class instance
 @return The CWeaponInfo* value
 */
CWeaponInfo* CEnemy3D::GetWeapon(void) const
{
	if (iCurrentWeapon == 0)
		return pPrimaryWeapon;
	else if (iCurrentWeapon == 1)
		return pSecondaryWeapon;

	return NULL;
}

/**
 @brief Set current weapon
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 */
void CEnemy3D::SetCurrentWeapon(const int iSlot)
{
	iCurrentWeapon = iSlot;
}

/**
 @brief Discharge weapon
 @return A bool value
 */
bool CEnemy3D::DischargeWeapon(void) const
{
	if ((iCurrentWeapon == 0) && (pPrimaryWeapon))
	{
		return pPrimaryWeapon->Discharge(vec3Position, vec3Front, (CSolidObject*)this);
	}
	else if ((iCurrentWeapon == 1) && (pSecondaryWeapon))
	{
		return pSecondaryWeapon->Discharge(vec3Position, vec3Front, (CSolidObject*)this);
	}
	return false;
}

/**
 @brief Processes input received from any keyboard-like input system as player movements. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
 @param direction A const Player_Movement variable which contains the movement direction of the camera
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CEnemy3D::ProcessMovement(const ENEMYMOVEMENT direction, const float deltaTime)
{
	float velocity = fMovementSpeed* deltaTime;
	if (direction == ENEMYMOVEMENT::FORWARD)
		vec3Position += vec3Front * velocity;
	if (direction == ENEMYMOVEMENT::BACKWARD)
		vec3Position -= vec3Front * velocity;
	if (direction == ENEMYMOVEMENT::LEFT)
		vec3Position -= vec3Right * velocity;
	if (direction == ENEMYMOVEMENT::RIGHT)
		vec3Position += vec3Right * velocity;

	// If the camera is attached to this player, then update the camera
	if (pCamera)
	{
		pCamera->vec3Position = vec3Position;
	}

	// Constraint the player's position
	Constraint();
}

/**
 @brief Processes input received from a mouse input system as player rotation. Expects the offset value in both the x and y direction.
 @param xoffset A const float variable which contains the x axis of the mouse movement
 */
void CEnemy3D::ProcessRotate(const float fXOffset)
{
	// Update the yaw
	fYaw += fXOffset;// *fRotationSensitivity;

	// Update vec3Front, Right and vec3Up Vectors using the updated Euler angles
	UpdateEnemyVectors();
}

/**
 @brief Update the elapsed time
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CEnemy3D::Update(const double dElapsedTime)
{
	// Don't update if this entity is not active
	if (bStatus == false)
	{
		return false;
	}

	// Store the enemy's current position, if rollback is needed.
	StorePositionForRollback();

	switch (sCurrentFSM)
	{
	case FSM::IDLE:
		if (iFSMCounter > iMaxFSMCounter)
		{
			sCurrentFSM = FSM::PATROL;
			iFSMCounter = 0;
			if (_DEBUG_FSM == true)
				cout << "Rested: Switching to Patrol State" << endl;
		}
		iFSMCounter++;
		break;
	case FSM::PATROL:
		// Check if the destination position has been reached
		if (pWaypointManager->HasReachedWayPoint(vec3Position))
		{
			vec3Front = glm::normalize((pWaypointManager->GetNextWaypoint()->GetPosition() - vec3Position));
			UpdateFrontAndYaw();

			if (_DEBUG_FSM == true)
				cout << "Reached waypoint: Going to next waypoint" << endl;
		}
		else if (iFSMCounter > iMaxFSMCounter)
		{
			sCurrentFSM = FSM::IDLE;
			iFSMCounter = 0;
			if (_DEBUG_FSM == true)
				cout << "FSM Counter maxed out: Switching to Idle State" << endl;
		}
		else if (glm::distance(vec3Position, pPlayer3D->GetPosition()) < fDetectionDistance)
		{
			sCurrentFSM = FSM::ATTACK;
			iFSMCounter = 0;
			if (_DEBUG_FSM == true)
				cout << "Target found: Switching to Attack State" << endl;
		}
		else
		{
			// Process the movement
			ProcessMovement(ENEMYMOVEMENT::FORWARD, (float)dElapsedTime);
			if (_DEBUG_FSM == true)
				cout << "Patrolling" << endl;
		}
		iFSMCounter++;
		break;
	case FSM::ATTACK:
		if (glm::distance(vec3Position, pPlayer3D->GetPosition()) < fDetectionDistance)
		{
			vec3Front = glm::normalize((pPlayer3D->GetPosition() - vec3Position));
			UpdateFrontAndYaw();

			// Discharge weapon
			if (DischargeWeapon() == false)
			{
				// Check if the weapon mag is empty
				if (pPrimaryWeapon->GetMagRound() == 0)
				{
					if (pPrimaryWeapon->GetTotalRound() != 0)
					{
						// Reload the weapon
						pPrimaryWeapon->Reload();
					}
				}
			}

			// Process the movement
			ProcessMovement(ENEMYMOVEMENT::FORWARD, (float)dElapsedTime);
			if (_DEBUG_FSM == true)
				cout << "Attacking now" << endl;
		}
		else
		{
			// If NPC loses track of player, then go back to the nearest waypoint
			vec3Front = glm::normalize((pWaypointManager->GetNearestWaypoint(vec3Position)->GetPosition() - vec3Position));
			UpdateFrontAndYaw();

			// Swtich to patrol mode
			sCurrentFSM = FSM::PATROL;
			//iFSMCounter = 0;
			if (_DEBUG_FSM == true)
				cout << "Switching to Patrol State" << endl;
		}
		iFSMCounter++;
		break;
	default:
		break;
	}

	// Update the model
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, vec3Position);
	model = glm::scale(model, vec3Scale);
	model = glm::rotate(model, glm::radians(fYaw), glm::vec3(0.0f, 1.0f, 0.0f));

	// Update the weapon's position
	if (pPrimaryWeapon)
	{
		//pPrimaryWeapon->SetPosition(vec3Position + glm::vec3(0.05f, -0.075f, 0.5f));
		pPrimaryWeapon->Update(dElapsedTime);
		glm::mat4 gunModel = model;
		gunModel = glm::rotate(gunModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		gunModel = glm::translate(gunModel, glm::vec3(0.05f, -0.075f, 0.5f));
		pPrimaryWeapon->SetModel(gunModel);
	}
	if (pSecondaryWeapon)
	{
		pSecondaryWeapon->SetPosition(vec3Position + glm::vec3(0.05f, -0.075f, 0.5f));
		pSecondaryWeapon->SetRotation(fYaw, glm::vec3(0.0f, 1.0f, 0.0f));
		pSecondaryWeapon->Update(dElapsedTime);
	}

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CEnemy3D::PreRender(void)
{
	// If this entity is not active, then skip this
	if (bStatus == false)
	{
		return;
	}

	CSolidObject::PreRender();
}

/**
 @brief Render Render this instance
 */
void CEnemy3D::Render(void)
{
	// If this entity is not active, then skip this
	if (bStatus == false)
	{
		return;
	}

	CSolidObject::Render();

	pPrimaryWeapon->SetView(view);
	pPrimaryWeapon->SetProjection(projection);
	pPrimaryWeapon->PreRender();
	pPrimaryWeapon->Render();
	pPrimaryWeapon->PostRender();
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CEnemy3D::PostRender(void)
{
	// If this entity is not active, then skip this
	if (bStatus == false)
	{
		return;
	}

	CSolidObject::PostRender();
}

/**
 @brief Calculates the front vector from the Camera's (updated) Euler Angles
 */
void CEnemy3D::UpdateEnemyVectors(void)
{
	// Calculate the new vec3Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front.y = sin(glm::radians(fPitch));
	front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front = glm::normalize(front);
	
	vec3Front = front;
	// Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more 
	// you look up or down which results in slower movement.
	vec3Right = glm::normalize(glm::cross(vec3Front, vec3WorldUp));  
	vec3Up = glm::normalize(glm::cross(vec3Right, vec3Front));

	// If the camera is attached to this player, then update the camera
	if (pCamera)
	{
		pCamera->vec3Front = vec3Front;
		pCamera->vec3Right = vec3Right;
		pCamera->vec3Up = vec3Up;
	}
}

/**
 @brief Constraint the player's position
 */
void CEnemy3D::Constraint(void)
{
	// Get the new height
	float fNewYValue = pTerrainManager->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;
	// Smooth out the change in height
	vec3Position.y += (fNewYValue - vec3Position.y) * 0.05f;
}

/**
 @brief Update Front Vector and Yaw
 */
void CEnemy3D::UpdateFrontAndYaw(void)
{
	fYaw = glm::degrees(glm::acos(dot(glm::vec3(1.0f, 0.0f, 0.0f), vec3Front)));
	if (cross(glm::vec3(1.0f, 0.0f, 0.0f), vec3Front).y < 0.0f)
		fYaw *= -1;
}
