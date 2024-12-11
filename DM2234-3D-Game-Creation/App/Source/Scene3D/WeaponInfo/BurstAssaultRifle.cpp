/**
 CPistol
 By: Toh Da Jun
 Date: Sep 2020
 */
#include "BurstAssaultRifle.h"

 // Include LoadOBJ
#include "System/LoadOBJ.h"

 // Include ImageLoader
#include "System\ImageLoader.h"

#include <iostream>
using namespace std;

/**
@brief Default Constructor
*/
CBurstAssaultRifle::CBurstAssaultRifle(void)
{
}

/**
@brief Default Destructor
*/
CBurstAssaultRifle::~CBurstAssaultRifle(void)
{
}

/**
 @brief Initialise this instance to default values
 */
bool CBurstAssaultRifle::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	iMagRounds = 30;
	// The maximum number of ammunition for this magazine for this weapon
	iMaxMagRounds = 30;
	// The current total number of rounds currently carried by this player
	iTotalRounds = 150;
	// The max total number of rounds currently carried by this player
	iMaxTotalRounds = 150;

	// The maximum elapsed time for reloading of a magazine in milliseconds
	dMaxReloadTime = 3.0f;

	iDamage = 20;

	isBurst = false;

	iBurstCount = 3;
	iShot = 0;
	dBurstDelay = 0.03;
	dCooldown = 0.3;

	fScopeMult = 2.f;

	fPitchRecoil = 1.f;
	fYawRecoil = 1.f;

	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<ModelVertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	std::string file_path = "Models/AK47/ak47.obj";
	bool success = CLoadOBJ::LoadOBJ(file_path.c_str(), vertices, uvs, normals, true);
	if (!success)
	{
		cout << "Unable to load Models/AK47/ak47.obj" << endl;
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
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Models/AK47/ak47.png", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Models/AK47/ak47.png" << endl;
		return false;
	}

	return true;
}

bool CBurstAssaultRifle::Update(const double dt)
{
	// Update the model matrix
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, glm::vec3(vec3Position.x, vec3Position.y, vec3Position.z));
	model = glm::scale(model, vec3Scale);
	model = glm::rotate(model, fRotationAngle, vec3RotationAxis);

	// If the weapon can fire, then just fire and return
	if (bFire) return false;
	
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
	else if (isBurst) {
		if (iShot > 0) {
			// Create a projectile. 
			// Its position is slightly in front of the player to prevent collision
			// Its direction is same as the player.
			// It will last for 2.0 seconds and travel at 20 units per frame
			CProjectileManager::GetInstance()->Activate(_pSource->GetPosition() + _pSource->GetFront() * 0.75f,
				_pSource->GetFront(),
				fLifeTime, fSpeed, iDamage, _pSource);
			ApplyRecoil();
			// Reset the dElapsedTime to dTimeBetweenShots for the next shot
			dTimeBetweenShots = dBurstDelay;
			// Reduce the rounds by 1
			iMagRounds--;
			iShot--;

			// Lock the weapon after this discharge
			bFire = false;
		}
		else {
			dTimeBetweenShots = dCooldown;
			isBurst = false;
		}
		dElapsedTime = dTimeBetweenShots;
	}
	else
	{
		// Set the weapon to fire ready since reloading is completed
		bFire = true;
	}

	return false;
}

bool CBurstAssaultRifle::Discharge(glm::vec3 vec3Position,
	glm::vec3 vec3Front,
	CSolidObject* pSource)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (iMagRounds > 0)
		{
			_pSource = pSource;
			iShot = iBurstCount;
			isBurst = true;

			bFire = false;

			return true;
		}
		else if (iMagRounds <= 0 && iTotalRounds > 0) {
			Reload();
		}
	}

	//cout << "Unable to discharge weapon." << endl;

	return false;
}