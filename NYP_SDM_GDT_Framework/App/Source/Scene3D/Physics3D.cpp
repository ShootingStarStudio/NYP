/**
 CPhysics3D
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "Physics3D.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CPhysics3D::CPhysics3D(void)
	: vec3InitialVelocity(glm::vec3(0.0f))
	, vec3FinalVelocity(glm::vec3(0.0f))
	, vec3Acceleration(glm::vec3(0.0f))
	, vec3PrevDisplacement(glm::vec3(0.0f))
	, vec3Displacement(glm::vec3(0.0f))
	, fTotalTime(0.0f)
	, fElapsedTime(0.0f)
	, sCurrentStatus(STATUS::IDLE)
	, bNewJump(false)
{
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CPhysics3D::~CPhysics3D(void)
{
}

/**
@brief Init Initialise this instance
@return true if the initialisation is successful, else false
*/
bool CPhysics3D::Init(void)
{
	// Reset these variables
	vec3InitialVelocity = glm::vec3(0.0f);
	vec3FinalVelocity = glm::vec3(0.0f);
	vec3Acceleration = glm::vec3(0.0f);		// Acceleration does not need to be reset here.
	vec3PrevDisplacement = glm::vec3(0.0f);
	vec3Displacement = glm::vec3(0.0f);
	fTotalTime = 0.0f;
	fElapsedTime = 0.0f;
	return true;
}

// Set methods
/**
 @brief Set Initial velocity
 @param vec3InitialVelocity A const glm::vec3 variable describing the initial velocity
 */
void CPhysics3D::SetInitialVelocity(const glm::vec3 vec3InitialVelocity)
{
	this->vec3InitialVelocity = vec3InitialVelocity;	// Initial velocity
}

/**
 @brief Set Final velocity
 @param vec3FinalVelocity A const glm::vec3 variable describing the final velocity
 */
void CPhysics3D::SetFinalVelocity(const glm::vec3 vec3FinalVelocity)
{
	this->vec3FinalVelocity = vec3FinalVelocity;		// Final velocity
}

/**
 @brief Set Acceleration
 @param vec3Acceleration A const glm::vec3 variable describing the acceleration
 */
void CPhysics3D::SetAcceleration(const glm::vec3 vec3Acceleration)
{
	this->vec3Acceleration = vec3Acceleration;		// Acceleration
}

/**
 @brief Set Displacement
 @param vec3Displacement A const glm::vec3 variable describing the displacement
 */
void CPhysics3D::SetDisplacement(const glm::vec3 vec3Displacement)
{
	this->vec3Displacement = vec3Displacement;		// Displacement
}

/**
 @brief Set Total Elapsed Time
 @param fTotalTime A const float variable describing the total time since this physics calculation started
 */
void CPhysics3D::SetTotalElapsedTime(const float fTotalTime)
{
	this->fTotalTime = fTotalTime;	// Total Elapsed Time
}

/**
 @brief Set Elapsed Time
 @param fElapsedTime A const float variable containing the elapsed time since the last frame
 */
void CPhysics3D::SetElapsedTime(const float fElapsedTime)
{
	this->fElapsedTime = fElapsedTime;	// Total Elapsed Time
}

/**
 @brief Set Status
 @param sStatus A const STATUS variable setting the new status of this class instance
 */
void CPhysics3D::SetStatus(const STATUS sStatus)
{
	// If there is a change in status, then reset to default values
	if (sCurrentStatus != sStatus)
	{
		// Reset to default values
		Init();

		// Store the new status
		sCurrentStatus = sStatus;
	}
}

/**
 @brief Set bNewJump
 */
void CPhysics3D::SetNewJump(const bool bNewJump)
{
	this->bNewJump = bNewJump;
}

// Get methods
/**
 @brief Get Initial velocity
 @return a glm::vec3 variable
*/
glm::vec3 CPhysics3D::GetInitialVelocity(void) const
{
	return vec3InitialVelocity;	// Initial velocity
}

/**
 @brief Get Final velocity
 @return a glm::vec3 variable
 */
glm::vec3 CPhysics3D::GetFinalVelocity(void) const
{
	return vec3FinalVelocity;		// Final velocity
}

/**
 @brief Get Acceleration
 @return a glm::vec3 variable
 */
glm::vec3 CPhysics3D::GetAcceleration(void) const
{
	return vec3Acceleration;		// Acceleration
}

/**
 @brief Get Displacement
 @return a glm::vec3 variable
 */
glm::vec3 CPhysics3D::GetDisplacement(void) const
{
	return vec3Displacement;		// Displacement
}

/**
 @brief Get Delta Displacement
 @return a glm::vec3 variable
 */
glm::vec3 CPhysics3D::GetDeltaDisplacement(void) const
{
	return vec3Displacement - vec3PrevDisplacement;		// Delta Displacement
}

/**
 @brief Get Time
 @return a float variable
 */
float CPhysics3D::GetTotalElapsedTime(void) const
{
	return fTotalTime;	// Total Elapsed Time
}

/**
 @brief Get Status
 @return a CPhysics3D::STATUS variable
 */
CPhysics3D::STATUS CPhysics3D::GetStatus(void) const
{
	return sCurrentStatus;
}

/**
 @brief Get bNewJump
 */
bool CPhysics3D::GetNewJump(void) const
{
	return bNewJump;
}

/**
 @brief Update
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CPhysics3D::Update(const double dElapsedTime)
{
	// If the player is in IDLE mode, 
	// then we don't calculate further
	if (sCurrentStatus == STATUS::IDLE)
		return false;

	// For a new jump, we skip the assigning of vec2FinalVelocity to vec2InitialVelocity
	if (bNewJump == true)
	{
		// And we set bNewJump to true
		bNewJump = false;
	}
	else
	{
		// Store the initial velocity to the final velocity
		vec3InitialVelocity = vec3FinalVelocity;
	}

	// Calculate the final velocity
	vec3FinalVelocity = vec3InitialVelocity + float(dElapsedTime) * vec3Gravity;
	// Calculate the change in position
	vec3Displacement = float(dElapsedTime) * vec3FinalVelocity;

	// Store the initial velocity to the final velocity
	//vec3InitialVelocity = vec3FinalVelocity;

	return true;
}

/**
 @brief Add elapsed time
 @param fElapsedTime A const float variable containing the elapsed time since the last frame
 */
void CPhysics3D::AddElapsedTime(const float fElapsedTime)
{
	this->fElapsedTime = fElapsedTime;
	fTotalTime += fElapsedTime;
}

/**
 @brief Calculate the distance between two vec2 varables
 @param source A const glm::vec3 variable containing the source position
 @param destination A const glm::vec3 variable containing the destination position
 @return a float variable
*/
float CPhysics3D::CalculateDistance(glm::vec3 source, glm::vec3 destination)
{
	return glm::length(destination - source);
}

/**
 @brief PrintSelf
 */
void CPhysics3D::PrintSelf(void)
{
	cout << "CPhysics3D::PrintSelf()" << endl;
	cout << "vec3InitialVelocity\t=\t[" << vec3InitialVelocity.x << ", " << vec3InitialVelocity.y << "]" << endl;
	cout << "vec3FinalVelocity\t=\t[" << vec3FinalVelocity.x << ", " << vec3FinalVelocity.y << "]" << endl;
	cout << "vec3Acceleration\t=\t[" << vec3Acceleration.x << ", " << vec3Acceleration.y << "]" << endl;
	cout << "vec3Displacement\t=\t[" << vec3Displacement.x << ", " << vec3Displacement.y << "]" << endl;
	cout << "fTotalTime\t=\t" << fTotalTime << endl;
	cout << "fElapsedTime\t=\t" << fElapsedTime << endl;
}
