/**
 CSolidObject
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "SolidObject.h"

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

// Include ImageLoader
#include "System\ImageLoader.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CSolidObject::CSolidObject(void)
	:fYaw(-90.0f)
	, fPitch(0.0f)
	, TargetEntity(nullptr)
	, fDetectionDistance(10.f)
	, cTerrain(NULL)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, 0.0f, 0.0f);
}

/**
 @brief Constructor with vectors
 @param vec3Position A const glm::vec3 variable which contains the position of the camera
 @param vec3Front A const glm::vec3 variable which contains the up direction of the camera
 */
CSolidObject::CSolidObject(	const glm::vec3 vec3Position,
							const glm::vec3 vec3Front)
	:fYaw(-90.0f)
	, fPitch(0.0f)
	, TargetEntity(nullptr)
	, fDetectionDistance(10.f)
	, cTerrain(NULL)
{
	// Set the default position to the origin
	this->vec3Position = vec3Position;
	this->vec3Front = vec3Front;
}

/**
 @brief Destructor
 */
CSolidObject::~CSolidObject(void)
{
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CSolidObject::Init(void)
{
	// Call the parent's Init()
	CEntity3D::Init();

	cTerrain = CTerrain::GetInstance();


	return true;;
}

/**
 @brief Initialise the collider
 @param _name A const std::string& variable which stores the name of the shader
 @param boxColour A glm::vec4 variable which is the colour of the collider box
 @param boxMin A glm::vec4 variable which is the minimum size of the collider box
 @param boxMax A glm::vec4 variable which is the maximum size of the collider box
 @return true is successfully initialised this class instance, else false
 */
bool CSolidObject::InitCollider(const std::string& _name,
								glm::vec4 boxColour,
								glm::vec3 boxMin,
								glm::vec3 boxMax)
{
	return CCollider::InitCollider(_name, boxColour, boxMin, boxMax);
}

/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void CSolidObject::SetModel(const glm::mat4 model)
{
	CEntity3D::model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CSolidObject::SetView(const glm::mat4 view)
{
	CEntity3D::view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CSolidObject::SetProjection(const glm::mat4 projection)
{
	CEntity3D::projection = projection;
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CSolidObject::Update(const double dElapsedTime)
{
	// create transformations
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, vec3Position);
	model = glm::scale(model, vec3Scale);
	//model = glm::rotate(model, glm::radians(fYaw), glm::vec3(0.0f, 1.0f, 0.0f));

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CSolidObject::PreRender(void)
{
	// change depth function so depth test passes when values are equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);  

	// Activate shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
 @brief Render Render this instance
 */
void CSolidObject::Render(void)
{
	// note: currently we set the projection matrix each frame, but since the projection 
	// matrix rarely changes it's often best practice to set it outside the main loop only once.
	CShaderManager::GetInstance()->activeShader->setMat4("projection", projection);
	CShaderManager::GetInstance()->activeShader->setMat4("view", view);
	CShaderManager::GetInstance()->activeShader->setMat4("model", model);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iTextureID);
		// Render the mesh
		glBindVertexArray(VAO);
			if (mesh)
				mesh->Render();
			else
				glDrawElements(GL_TRIANGLES, iIndicesSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Render the CCollider if needed
	if (this->bIsDisplayed)
	{
		CCollider::colliderModel = CEntity3D::model;
		CCollider::colliderView = CEntity3D::view;
		CCollider::colliderProjection = CEntity3D::projection;
		CCollider::PreRender();
		CCollider::Render();
		CCollider::PostRender();
	}
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CSolidObject::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

/**
 @brief Print Self
 */
void CSolidObject::PrintSelf(void)
{
	cout << "CSolidObject::PrintSelf()" << endl;
	cout << "========================" << endl;
}

float CSolidObject::getYaw(void) {
	return fYaw;
}
void CSolidObject::setYaw(float yaw) {
	fYaw = yaw;
}
float CSolidObject::getPitch(void) {
	return fPitch;
}
void CSolidObject::setPitch(float pitch) {
	fPitch = pitch;
}

float CSolidObject::getDetectionDistance(void) {
	return fDetectionDistance;
}
void CSolidObject::setDetectionDistance(float distance) {
	fDetectionDistance = distance;
}

/**
 @brief Set to Jump
 */
void CSolidObject::SetToJump(void)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
	{
		cPhysics3D.SetStatus(CPhysics3D::STATUS::JUMP);
		cPhysics3D.SetInitialVelocity(glm::vec3(0.0f, 1.2f, 0.0f));
	}
}

/**
 @brief Set to Fall
 */
void CSolidObject::SetToFall(void)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::JUMP)
	{
		cPhysics3D.SetStatus(CPhysics3D::STATUS::FALL);
	}
	else if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
	{
		cPhysics3D.SetStatus(CPhysics3D::STATUS::FALL);
	}
}

/**
 @brief Set to Idle
 */
void CSolidObject::SetToIdle(void)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::FALL)
	{
		cPhysics3D.SetStatus(CPhysics3D::STATUS::IDLE);
	}
	else if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::FALL)
	{
		cPhysics3D.SetStatus(CPhysics3D::STATUS::IDLE);
	}
}


/**
 @brief Constraint the player's position
 */
void CSolidObject::Constraint(void)
{
	// If the player is not jumping nor falling, then we snap his position to the terrain
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
	{
		vec3Position.y = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;
	}
	else
	{
		// If the player is jumping nor falling, then we only snap his position 
		// if the player's y-coordinate is below the cTerrain 
		float fCheckHeight = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;

		// If the player is below the terrain, then snap to the terrain height
		if (fCheckHeight > vec3Position.y)
			vec3Position.y = fCheckHeight;
	}
}

/**
 @brief Update Jump or Fall
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CSolidObject::UpdateJumpFall(const double dElapsedTime)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::JUMP)
	{
		// Update the elapsed time to the physics engine
		cPhysics3D.AddElapsedTime((float)dElapsedTime);
		// Call the physics engine update method to calculate the final velocity and displacement
		cPhysics3D.Update(dElapsedTime);
		// Get the displacement from the physics engine and update the player position
		vec3Position = vec3Position + cPhysics3D.GetDisplacement();

		// If the player is still jumping and the initial velocity has reached zero or below zero, 
		// then it has reach the peak of its jump
		if ((cPhysics3D.GetStatus() == CPhysics3D::STATUS::JUMP) && (cPhysics3D.GetDeltaDisplacement().y <= 0.0f))
		{
			// Set status to fall
			cPhysics3D.SetStatus(CPhysics3D::STATUS::FALL);
		}
	}
	else if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::FALL)
	{
		// Update the elapsed time to the physics engine
		cPhysics3D.AddElapsedTime((float)dElapsedTime);
		// Call the physics engine update method to calculate the final velocity and displacement
		cPhysics3D.Update(dElapsedTime);
		// Get the displacement from the physics engine and update the player position
		vec3Position = vec3Position + cPhysics3D.GetDisplacement();

		// Constaint the player's position to the terrain
		float fCheckHeight = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;

		// Set the Physics to idle status
		if (fCheckHeight > vec3Position.y)
		{
			vec3Position.y = fCheckHeight;
			cPhysics3D.SetStatus(CPhysics3D::STATUS::IDLE);
		}
	}
}

// Set the pointer of the terrain to this player3D class
void CSolidObject::SetTerrain(CTerrain* theNewTerrain)
{
	cTerrain = theNewTerrain;

	if (cTerrain != NULL)
	{
		// Update the y-axis position of the player
		vec3Position.y = cTerrain->GetHeight(vec3Position.x, vec3Position.z);
	}
}

int CSolidObject::getID(void) {
	return 0;
}
float CSolidObject::getValue(void) {
	return 0;
}