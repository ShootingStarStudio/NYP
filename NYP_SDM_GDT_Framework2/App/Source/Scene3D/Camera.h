/**
 CCamera: An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include <string>
using namespace std;

// Default camera values
const float YAW         = 0.0f;
const float PITCH       =  0.0f;
const float SPEED       =  10.0f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class CCamera : public CSingletonTemplate<CCamera>
{
	friend class CSingletonTemplate<CCamera>;

public:
	// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
	enum class CAMERAMOVEMENT : unsigned int
	{
		FORWARD = 0,
		BACKWARD,
		LEFT,
		RIGHT,
		NUM_MOVEMENTS
	};

	// Defines several possible modes for camera views. Used as abstraction to stay away from window-system specific input methods
	enum class CAMERAVIEW : unsigned int
	{
		FIRSTPERSONVIEW = 0,	// First person view
		FRONTVIEW,
		REARVIEW,
		LEFTVIEW,
		RIGHTVIEW,
		TOPVIEW,
		BOTTOMVIEW,
		ISOVIEW,
		THIRDPERSONVIEW,
		NUM_VIEWS
	};
	// The distance to offset the camera from the vec3ReferencePosition supplied to SetCameraView() method
	const float fCameraViewOffset = 5.0f;

    // Camera Attributes
    glm::vec3 vec3Position;
    glm::vec3 vec3Front;
    glm::vec3 vec3Up;
    glm::vec3 vec3Right;
    glm::vec3 vec3WorldUp;
	glm::vec3 vec3Offset;
	// Euler Angles
    float fYaw;
    float fPitch;
    // Camera options
    float fMovementSpeed;
    float fMouseSensitivity;
    float fZoom;

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix(void) const;
	// Returns the Perspective Matrix
	glm::mat4 GetPerspectiveMatrix(void) const;

	// Set the current camera view
	bool SetCameraView(const CAMERAVIEW sCameraView, glm::vec3 vec3ReferencePosition, glm::vec3 vec3ReferenceFront = glm::vec3(0.0f, 0.0f, 1.0f));

	// Get the current camera view
	CAMERAVIEW GetCameraView(void) const;
	// Get the current camera view name
	string GetCameraViewName(void) const;

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(const CAMERAMOVEMENT direction, const float fDeltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float fXOffset, float fYOffset, const bool bConstrainPitch = true);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(const float fYOffset);

	// PrintSelf
	void PrintSelf(void);

protected:
	// The variable storing the current camera view
	CAMERAVIEW sCurrentCameraView;

	// Default Constructor
	CCamera(void);

	// Constructor with vectors
	CCamera(const glm::vec3 position,
			const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			const float fYaw = YAW,
			const float fPitch = PITCH);

	// Constructor with scalar values
	CCamera(const float fPosX, const float fPosY, const float fPosZ,
			const float fUpX, const float fUpY, const float fUpZ,
			const float fYaw, const float fPitch);

	// Destructor
	virtual ~CCamera(void);

    // Calculates the front vector from the Camera's (updated) Euler Angles
	void UpdateCameraVectors(void);

	// Calculates the yaw and pitch from a provided front vector
	void CalculateYawPitch(glm::vec3 vec3Front, float& fYaw, float& fPitch);
};
