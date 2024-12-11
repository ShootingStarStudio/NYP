/**
 CLightMachineGun
 By: Toh Da Jun
 Date: Sep 2020
 */
#include "MiniGun.h"

 // Include LoadOBJ
#include "System/LoadOBJ.h"

 // Include ImageLoader
#include "System\ImageLoader.h"

#include <iostream>
using namespace std;

/**
@brief Default Constructor
*/
CMiniGun::CMiniGun(void)
{
}

/**
@brief Default Destructor
*/
CMiniGun::~CMiniGun(void)
{
}

/**
 @brief Initialise this instance to default values
 */
bool CMiniGun::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	iMagRounds = 100;
	// The maximum number of ammunition for this magazine for this weapon
	iMaxMagRounds = 100;
	// The current total number of rounds currently carried by this player
	iTotalRounds = 500;
	// The max total number of rounds currently carried by this player
	iMaxTotalRounds = 500;

	// The maximum elapsed time for reloading of a magazine in milliseconds
	dMaxReloadTime = 5.0;

	fPitchRecoil = 1.f;
	fYawRecoil = 2.0f;

	iDamage = 10;

	// Default firing rate
	dBaseFiringRate = 0.3;
	// Max firing rate
	dMaxFiringRate = 0.06;
	// Acceleration
	dAcceleration = 0.03;
	//time before the gun's firing rate resets
	dCoolDown = 1.0;

	fScopeMult = 1.5f;

	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<ModelVertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	std::string file_path = "Models/Pistol/Pistol.obj";
	bool success = CLoadOBJ::LoadOBJ(file_path.c_str(), vertices, uvs, normals, true);
	if (!success)
	{
		cout << "Unable to load Models/Pistol/Pistol.obj" << endl;
		return false;
	}

	CLoadOBJ::IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(ModelVertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	iIndicesSize = index_buffer_data.size();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec3)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Models/Pistol/Pistol.png", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Models/Pistol/Pistol.png" << endl;
		return false;
	}

	return true;
}

bool CMiniGun::Update(const double dt)
{
	// Update the model matrix
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, glm::vec3(vec3Position.x, vec3Position.y, vec3Position.z));
	model = glm::scale(model, vec3Scale);
	model = glm::rotate(model, fRotationAngle, vec3RotationAxis);


	// If the weapon can fire, then just fire and return
	if (bFire) {
		dLastFired += dt;
		if (dLastFired >= dCoolDown) {
			dTimeBetweenShots = dBaseFiringRate;
		}
		return false;
	}

	// Update the dReloadTime
	if (dReloadTime >= 0.0f)
	{
		// Reduce the dReloadTime
		dReloadTime -= dt;
		// Return true since we have already updated the dReloadTime
		return true;
	}
	// Update the elapsed time if there is no reload to countdown
	else if (dElapsedTime > 0.0f)
	{
		dElapsedTime -= dt;
		// Return true since we have already updated the dReloadTime
		return true;
	}
	else
	{
		// Set the weapon to fire ready since reloading is completed
		bFire = true;
	}

	return false;
}

bool CMiniGun::Discharge(glm::vec3 vec3Position,
	glm::vec3 vec3Front,
	CSolidObject* pSource)
{
	if (bFire && iMagRounds > 0)
	{
		// If there is still ammo in the magazine, then fire

			// Create a projectile. 
			// Its position is slightly in front of the player to prevent collision
			// Its direction is same as the player.
			// It will last for 2.0 seconds and travel at 20 units per frame
		CProjectileManager::GetInstance()->Activate(vec3Position + vec3Front * 0.75f,
			vec3Front,
			fLifeTime, fSpeed, iDamage, pSource);
		ApplyRecoil();
		if (dTimeBetweenShots > dMaxFiringRate) {
			dTimeBetweenShots -= dAcceleration;
		}

		// Lock the weapon after this discharge
		bFire = false;
		// Reset the dElapsedTime to dTimeBetweenShots for the next shot
		dElapsedTime = dTimeBetweenShots;
		// Reduce the rounds by 1
		iMagRounds--;

		dLastFired = 0.0;

		return true;
	}
	else if (iMagRounds <= 0 && iTotalRounds > 0) {
		Reload();
	}

	//cout << "Unable to discharge weapon." << endl;

	return false;
}
