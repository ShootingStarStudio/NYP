/**
 CPistol
 By: Toh Da Jun
 Date: Sep 2020
 */
#include "Pistol.h"

 // Include LoadOBJ
#include "System/LoadOBJ.h"

 // Include ImageLoader
#include "System\ImageLoader.h"

#include <iostream>
using namespace std;

/**
@brief Default Constructor
*/
CPistol::CPistol(void)
{
}

/**
@brief Default Destructor
*/
CPistol::~CPistol(void)
{
}

/**
 @brief Initialise this instance to default values
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CPistol::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	iMagRounds = 8;
	// The maximum number of ammunition for this magazine for this weapon
	iMaxMagRounds = 8;
	// The current total number of rounds currently carried by this player
	iTotalRounds = 40;
	// The max total number of rounds currently carried by this player
	iMaxTotalRounds = 40;

	// The time between shots
	dTimeBetweenShots = 0.3333;
	// The elapsed time (between shots)
	dElapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;

	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	std::string file_path = "Models/Pistol/gun_type64_01.obj";
	CLoadOBJ theCLoadOBJ;
	// Load the OBJ and store the key parameters into p3DMesh
	p3DMesh = theCLoadOBJ.CreateMesh(file_path.c_str(), true, true);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Models/Pistol/map_gunType64_01_AO_RGB.png", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Models/Pistol/map_gunType64_01_AO_RGB.png" << endl;
		return false;
	}

	// Load the specular map. 
	specularMap = CImageLoader::GetInstance()->LoadTextureGetID("Models/Pistol/map_gunType64_01_AO.png", false);
	if (specularMap == 0)
	{
		cout << "Unable to load Models/Pistol/map_gunType64_01_AO.png" << endl;
		return false;
	}

	// Since this class instance has been initialised successfully, then it is usable
	bIsUsable = true;

	return true;
}
