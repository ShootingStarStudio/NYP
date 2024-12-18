/**
 CPhysics3D
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

class CPhysics3D
{
public:
	// Physics statuses
	enum class STATUS : unsigned int
	{
		IDLE = 0,
		JUMP,
		FALL,
		NUM_STATUS
	};

	// Constructor
	CPhysics3D(void);

	// Destructor
	virtual ~CPhysics3D(void);

	// Init
	bool Init(void);

	// Set methods
	void SetInitialVelocity(const glm::vec3 vec3InitialVelocity);	// Set Initial velocity
	void SetFinalVelocity(const glm::vec3 vec3FinalVelocity);		// Set Final velocity
	void SetAcceleration(const glm::vec3 vec3Acceleration);		// Set Acceleration
	void SetDisplacement(const glm::vec3 vec3Displacement);		// Set Displacement
	void SetTotalElapsedTime(const float fTotalElapsedTime);	// Set Total Elapsed Time
	void SetElapsedTime(const float fElapsedTime);				// Set Elapsed Time
	void SetStatus(const STATUS sStatus);						// Set Status
	void SetNewJump(const bool bNewJump);						// Set bNewJump

	// Get methods
	glm::vec3 GetInitialVelocity(void) const;	// Get Initial velocity
	glm::vec3 GetFinalVelocity(void) const;		// Get Final velocity
	glm::vec3 GetAcceleration(void) const;		// Get Acceleration
	glm::vec3 GetDeltaDisplacement(void) const;	// Get Delta Displacement
	glm::vec3 GetDisplacement(void) const;		// Get Displacement
	float GetTotalElapsedTime(void) const;		// Get Total Elapsed Time
	STATUS GetStatus(void) const;				// Get Status
	bool GetNewJump(void) const;				// Get bNewJump


	// Update
	bool Update(const double dElapsedTime);

	// Add elapsed time
	void AddElapsedTime(const float fElapsedTime);

	// Calculate the distance between two vec2 varables
	float CalculateDistance(glm::vec3 source, glm::vec3 destination);

	// PrintSelf
	void PrintSelf(void);

protected:
	// Variables for SUVAT calculations
	glm::vec3 vec3InitialVelocity;	// Initial velocity
	glm::vec3 vec3FinalVelocity;		// Final velocity
	glm::vec3 vec3Acceleration;		// Acceleration
	glm::vec3 vec3PrevDisplacement;	// Previous Displacement
	glm::vec3 vec3Displacement;		// Displacement
	float fTotalTime;				// Total time since the motion was started
	float fElapsedTime;				// Elapsed time between the previous and current update

	// Gravity constant
	const glm::vec3 vec3Gravity = glm::vec3( 0.0f, -10.0f, 0.0f);

	// Current status
	STATUS sCurrentStatus;

	bool bNewJump;
};

