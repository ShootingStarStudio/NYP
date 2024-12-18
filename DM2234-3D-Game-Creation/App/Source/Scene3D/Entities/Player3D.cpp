/**
 CPlayer3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Player3D.h"

 // Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include ImageLoader
#include "System/ImageLoader.h"

//For allowing creating of Mesh 
#include "Primitives/MeshBuilder.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CPlayer3D::CPlayer3D(void)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fMouseSensitivity(0.1f)
	, cMouseController(NULL)
	, cKeyboardController(NULL)
	, cCamera(NULL)
	, cPrimaryWeapon(NULL)
	, cSecondaryWeapon(NULL)
	, iCurrentWeapon(0)
	, fCameraSwayAngle(0.0f)
	, fCameraSwayDeltaAngle(0.25f)
	, bCameraSwayDirection(false)	// false = left, true = right
	, bCameraSwayActive(true)
{
	// Set the default position so it is above the ground
	vec3Position = glm::vec3(0.0f, 0.5f, 0.0f);

	isIndestructable = true;

	// Update the player vectors
	UpdatePlayerVectors();
}

/**
 @brief Constructor with vectors
 @param position A const glm::vec3 variable which contains the position of the camera
 @param up A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CPlayer3D::CPlayer3D(	const glm::vec3 vec3Position,
						const glm::vec3 vec3Front,
						const float fYaw,
						const float fPitch)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fMouseSensitivity(0.1f)
	, cMouseController(NULL)
	, cKeyboardController(NULL)
	, cCamera(NULL)
	, cPrimaryWeapon(NULL)
	, cSecondaryWeapon(NULL)
	, iCurrentWeapon(0)
	, fCameraSwayAngle(0.0f)
	, fCameraSwayDeltaAngle(0.5f)
	, bCameraSwayDirection(false)	// false = left, true = right
	, bCameraSwayActive(true)
{
	mesh = NULL;

	this->vec3Position = vec3Position;
	this->vec3Front = vec3Front;
	this->fYaw = fYaw;
	this->fPitch = fPitch;

	isIndestructable = true;

	// Update the player vectors
	UpdatePlayerVectors();
}

/**
 @brief Destructor
 */
CPlayer3D::~CPlayer3D(void)
{
	if (cSecondaryWeapon)
	{
		delete cSecondaryWeapon;
		cSecondaryWeapon = NULL;
	}

	if (cPrimaryWeapon)
	{
		delete cPrimaryWeapon;
		cPrimaryWeapon = NULL;
	}

	if (cTerrain)
	{
		// We set it to NULL only since it was declared somewhere else
		cTerrain = NULL;
	}
	
	if (cCamera)
	{
		// We set it to NULL only since it was declared somewhere else
		cCamera = NULL;
	}

	// We won't delete this since it was created elsewhere
	cMouseController = NULL;

	// We won't delete this since it was created elsewhere
	cKeyboardController = NULL;
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CPlayer3D::Init(void)
{
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::PLAYER);

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//CS: Create the Quad Mesh using the mesh builder
	mesh = CMeshBuilder::GenerateBox(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		1.0f, 1.0f, 1.0f);

	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();

	// Store the mouse controller singleton instance here
	cMouseController = CMouseController::GetInstance();

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene3D_Player.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Player.tga" << endl;
		return false;
	}

	// Store the handler to the CTerrain
	cTerrain = CTerrain::GetInstance();
	if (cTerrain != NULL) {
		// Update the y-axis position of the player
		vec3Position.y = cTerrain->GetHeight(vec3Position.x, vec3Position.z);
	}

	// Set the Physics to fall status by default
	cPhysics3D.SetStatus(CPhysics3D::STATUS::FALL);

	// Set the postures
	iPreviousPosture = PLAYERPOSTURE::STANDING;
	iCurrentPosture = PLAYERPOSTURE::STANDING;
	iNextPosture = PLAYERPOSTURE::STANDING;
	fPostureOffset = 0.0f;

	Faction = 1;

	return true;
};

/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void CPlayer3D::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CPlayer3D::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CPlayer3D::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
@brief Returns the view matrix calculated using Euler Angles and the LookAt Matrix
@return A glm::mat4 variable which contains the view matrix
*/
glm::mat4 CPlayer3D::GetViewMatrix(void) const
{
	return glm::lookAt(vec3Position, vec3Position + vec3Front, vec3Up);
}

/**
 @brief Attach a camera to this class instance
 @param cCamera A CCamera* variable which contains the camera
 */
void CPlayer3D::AttachCamera(CCamera* cCamera)
{
	// Set the camera to the player
	this->cCamera = cCamera;

	// Update the camera's attributes with the player's attributes
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw;
		cCamera->fPitch = fPitch;
	}
}

/**
 @brief Check if a camera ia attached to this class instance
 @return true if a camera is attached, else false
 */
bool CPlayer3D::IsCameraAttached(void)
{
	if (cCamera)
		return true;
	return false;
}

/**
 @brief Set Weapon to this class instance
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 @param cWeaponInfo A CWeaponInfo* variable which contains the weapon info
 */
void CPlayer3D::SetWeapon(const int iSlot, CWeaponInfo* cWeaponInfo)
{
	if (iSlot == 0)
		cPrimaryWeapon = cWeaponInfo;
	else if (iSlot == 1)
		cSecondaryWeapon = cWeaponInfo;
}

/**
 @brief Get Weapon of this class instance
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 */
CWeaponInfo* CPlayer3D::GetWeapon(void) const
{
	if (iCurrentWeapon == 0)
		return cPrimaryWeapon;
	else if (iCurrentWeapon == 1)
		return cSecondaryWeapon;

	return NULL;
}

/**
 @brief Set current weapon
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 */
void CPlayer3D::SetCurrentWeapon(const int iSlot)
{
	iCurrentWeapon = iSlot;
}

/**
 @brief Discharge weapon
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 @return A bool variable
 */
bool CPlayer3D::DischargeWeapon(void) const
{
	if ((iCurrentWeapon == 0) && (cPrimaryWeapon))
	{
		return cPrimaryWeapon->Discharge(vec3Position, vec3Front, (CSolidObject*)this);
	}
	else if ((iCurrentWeapon == 1) && (cSecondaryWeapon))
	{
		return cSecondaryWeapon->Discharge(vec3Position, vec3Front, (CSolidObject*)this);
	}
	return NULL;
}

/**
 @brief Processes input received from any keyboard-like input system as player movements. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
 @param direction A const Player_Movement variable which contains the movement direction of the camera
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CPlayer3D::ProcessMovement(const PLAYERMOVEMENT direction, const float deltaTime)
{
	float velocity = fMovementSpeed * deltaTime;
	if ((vec3Position.y - fHeightOffset) < 12.5) {				//half speed swimming, lower than sea level
		velocity *= 0.5f;
	}
	else if (sprint) {							// twice speed when sprinting
		velocity *= 2.f;
	}
	else if (scoped) {						// half speed when scoped
		velocity *= 0.5f;
		velocity *= 0.5f;
	}
	else {
		// nil
	}
	
	if (direction == PLAYERMOVEMENT::FORWARD)
		vec3Position += vec3Front * velocity;
	if (direction == PLAYERMOVEMENT::BACKWARD)
		vec3Position -= vec3Front * velocity;
	if (direction == PLAYERMOVEMENT::LEFT)
		vec3Position -= vec3Right * velocity;
	if (direction == PLAYERMOVEMENT::RIGHT)
		vec3Position += vec3Right * velocity;

	// Indicate that camera sway is to be updated
	if (bCameraSwayActive)
		bUpdateCameraSway = true;

	// Snap to the terrain height
	//vec3Position.y = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;
}

/**
 @brief Processes input received from a mouse input system as player rotation. Expects the offset value in both the x and y direction.
 @param xoffset A const float variable which contains the x axis of the mouse movement
 @param yoffset A const float variable which contains the y axis of the mouse movement
 @param constrainPitch A const bool variable which indicates if the pitch will be constrained at upright positions
 */
void CPlayer3D::ProcessRotate(float fXOffset, float fYOffset, const bool constrainPitch)
{
	fXOffset *= fMouseSensitivity;
	fYOffset *= fMouseSensitivity;

	fYaw += fXOffset;
	fPitch += fYOffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (fPitch > 89.0f)
			fPitch = 89.0f;
		if (fPitch < -89.0f)
			fPitch = -89.0f;
	}

	// Update vec3Front, Right and vec3Up Vectors using the updated Euler angles
	UpdatePlayerVectors();
}

/**
 @brief Toggle the player's posture
 */
void CPlayer3D::TogglePosture(void)
{
	switch (iCurrentPosture)
	{
	case PLAYERPOSTURE::STANDING:
		iNextPosture = PLAYERPOSTURE::CROUCH;
		break;
	case PLAYERPOSTURE::CROUCH:
		if (iPreviousPosture == PLAYERPOSTURE::STANDING)
			iNextPosture = PLAYERPOSTURE::PRONE;
		else if (iPreviousPosture == PLAYERPOSTURE::PRONE)
			iNextPosture = PLAYERPOSTURE::STANDING;
		break;
	case PLAYERPOSTURE::PRONE:
		iNextPosture = PLAYERPOSTURE::CROUCH;
		break;
	default:
		break;
	}
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CPlayer3D::Update(const double dElapsedTime)
{
	//store the current position, if rollback is needed
	StorePositionForRollback();

	CSolidObject::Update(dElapsedTime);

	//weapon
	if (!scoped) {
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_1)) {
			SetCurrentWeapon(0);
		}
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_2)) {
			SetCurrentWeapon(1);
		}
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_R)) {
			GetWeapon()->Reload();
		}
	}

	if (cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::LMB)) {
		DischargeWeapon();
	}

	if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_LEFT_CONTROL)) {
		TogglePosture();
	}
	UpdatePosture(dElapsedTime);
	if (iCurrentPosture == PLAYERPOSTURE::STANDING) {
		//allow jumping only when standing
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_SPACE))
			SetToJump();
		//allow sprinting only when standing
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
			sprint = true;
			scoped = false;
		}
		else if (CKeyboardController::GetInstance()->IsKeyUp(GLFW_KEY_LEFT_SHIFT)) {
			sprint = false;
		}
	}
	else if (iCurrentPosture == PLAYERPOSTURE::CROUCH || iCurrentPosture == PLAYERPOSTURE::PRONE) {
		sprint = false;
	}
	if (cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::RMB)) scoped = true;
	else if (cMouseController->IsButtonUp(CMouseController::BUTTON_TYPE::RMB)) scoped = false;
	if (!sprint && scoped) {
		//CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(true);
		CCameraEffectsManager::GetInstance()->Get("ScopeScreen")->SetStatus(true);
		if (cCamera) {
			if (cCamera->fZoom > default_FOV * GetWeapon()->GetScopeMultiplier()) {
				cCamera->fZoom -= 10.0f;
			}
		}
	}
	else if (sprint || !scoped) {
		//CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(false);
		CCameraEffectsManager::GetInstance()->Get("ScopeScreen")->SetStatus(false);
		if (cCamera) {
			if (cCamera->fZoom < default_FOV) {
				cCamera->fZoom += 10.0f;
			}
		}
	}
	//weapon update
	if (cPrimaryWeapon)
		cPrimaryWeapon->Update(dElapsedTime);
	if (cSecondaryWeapon)
		cSecondaryWeapon->Update(dElapsedTime);

	// Update the Jump/Fall
	UpdateJumpFall(dElapsedTime);

	// Constraint the player's position
	Constraint();

	bool b_movement = false;

	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W)) {
		ProcessMovement(CPlayer3D::PLAYERMOVEMENT::FORWARD, (float)dElapsedTime);
		b_movement = true;
		CCameraEffectsManager::GetInstance()->Get("CameraShake")->SetStatus(true);
	}
	else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S)) {
		ProcessMovement(CPlayer3D::PLAYERMOVEMENT::BACKWARD, (float)dElapsedTime);
		b_movement = true;
		CCameraEffectsManager::GetInstance()->Get("CameraShake")->SetStatus(true);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A)) {
		ProcessMovement(CPlayer3D::PLAYERMOVEMENT::LEFT, (float)dElapsedTime);
		b_movement = true;
	}
	else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D)) {
		ProcessMovement(CPlayer3D::PLAYERMOVEMENT::RIGHT, (float)dElapsedTime);
		b_movement = true;
	}

	if (b_movement == false) {
		CCameraEffectsManager::GetInstance()->Get("CameraShake")->SetStatus(false);
	}

	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_Q))
	{
		CCameraEffectsManager::GetInstance()->Get("LeanLeft")->SetStatus(true);
	}
	else if (CKeyboardController::GetInstance()->IsKeyUp(GLFW_KEY_Q)) {
		CCameraEffectsManager::GetInstance()->Get("LeanLeft")->SetStatus(false);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_E))
	{
		CCameraEffectsManager::GetInstance()->Get("LeanRight")->SetStatus(true);
	}
	else if (CKeyboardController::GetInstance()->IsKeyUp(GLFW_KEY_E)) {
		CCameraEffectsManager::GetInstance()->Get("LeanRight")->SetStatus(false);
	}

	if ((bUpdateCameraSway) && (bCameraSwayActive))
	{
		glm::mat4 rotationMat(1); // Creates a identity matrix
		rotationMat = glm::rotate(rotationMat, glm::radians(fCameraSwayAngle), vec3Front);
		vec3Up = glm::vec3(rotationMat * glm::vec4(vec3WorldUp, 1.0f));
	}

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw;
		cCamera->fPitch = fPitch;
	}

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CPlayer3D::PreRender(void)
{
	CSolidObject::PreRender();
}

/**
 @brief Render Render this instance
 */
void CPlayer3D::Render(void)
{
	// Don't render the player if the camera is attached to it
	if (cCamera)
		return;

	CSolidObject::Render();
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CPlayer3D::PostRender(void)
{
	CSolidObject::PostRender();
}

/**
 @brief Calculates the front vector from the Camera's (updated) Euler Angles
 */
void CPlayer3D::UpdatePlayerVectors(void)
{
	// Calculate the new vec3Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front.y = sin(glm::radians(fPitch));
	front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	vec3Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more 
	// you look up or down which results in slower movement.
	vec3Right = glm::normalize(glm::cross(vec3Front, vec3WorldUp));  
	if (!bCameraSwayActive)
		vec3Up = glm::normalize(glm::cross(vec3Right, vec3Front));

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw;
		cCamera->fPitch = fPitch;
	}
}

/**
 @brief Update Posture
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CPlayer3D::UpdatePosture(const double dElapsedTime)
{
	// Return if there is no change in posture
	if (iCurrentPosture == iNextPosture)
		return;

	switch (iNextPosture)
	{
	case PLAYERPOSTURE::STANDING:
		fMovementSpeed = 5.f;
		fPostureOffset -= 0.01f;
		if (fPostureOffset < 0.0f)
		{
			fPostureOffset = 0.0f;
			iPreviousPosture = iCurrentPosture;
			iCurrentPosture = iNextPosture;
		}
		break;
	case PLAYERPOSTURE::CROUCH:
		fMovementSpeed = 3.5f;
		if (iCurrentPosture == PLAYERPOSTURE::STANDING)
		{
			fPostureOffset += 0.01f;
			if (fPostureOffset > 0.25f)
			{
				fPostureOffset = 0.25f;
				iPreviousPosture = iCurrentPosture;
				iCurrentPosture = iNextPosture;
			}
		}
		else if (iCurrentPosture == PLAYERPOSTURE::PRONE)
		{
			fPostureOffset -= 0.01f;
			if (fPostureOffset < 0.25f)
			{
				fPostureOffset = 0.25f;
				iPreviousPosture = iCurrentPosture;
				iCurrentPosture = iNextPosture;
			}
		}
		break;
	case PLAYERPOSTURE::PRONE:
		fMovementSpeed = 2.f;
		fPostureOffset += 0.01f;
		if (fPostureOffset > 0.35f)
		{
			fPostureOffset = 0.35f;
			iPreviousPosture = iCurrentPosture;
			iCurrentPosture = iNextPosture;
		}
		break;
	default:
		break;
	}
}

/**
 @brief Print Self
 */
void CPlayer3D::PrintSelf(void)
{
	cout << "CPlayer3D::PrintSelf()" << endl;
	cout << "========================" << endl;
}

void CPlayer3D::Constraint(void)
{
	// If the player is not jumping nor falling, then we snap his position to the terrain
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
	{
		vec3Position.y = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset - fPostureOffset;
	}
	else
	{
		// If the player is jumping nor falling, then we only snap his position 
		// if the player's y-coordinate is below the cTerrain 
		float fCheckHeight = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset - fPostureOffset;

		// If the player is below the terrain, then snap to the terrain height
		if (fCheckHeight > vec3Position.y)
			vec3Position.y = fCheckHeight;
	}
}

int CPlayer3D::getMaxHealth(void) {
	return MaxHealth;
}

void CPlayer3D::SetStatus(const bool bStatus)
{
	cout << "OH NOOO\n";
	this->bStatus = bStatus;
}