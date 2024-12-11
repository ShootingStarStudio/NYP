/**
 CSolidObjectManager
 By: Toh Da Jun
 Date: Apr 2020
 */

#include "SolidObjectManager.h"

// Include CCollisionManager
#include "Primitives/CollisionManager.h"

// Include CCameraEffectsManager
//#include "../CameraEffects/CameraEffectsManager.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CSolidObjectManager::CSolidObjectManager(void)
	: model(glm::mat4(1.0f))
	, view(glm::mat4(1.0f))
	, projection(glm::mat4(1.0f))
	, cProjectileManager(NULL)
	, cPlayer3D(NULL)
{
}

/**
 @brief Destructor
 */
CSolidObjectManager::~CSolidObjectManager(void)
{
	// We won't delete this since it was created elsewhere
	cProjectileManager = NULL;
	cPlayer3D = NULL;

	// Remove all CSolidObject
	std::list<CSolidObject*>::iterator it = lSolidObject.begin(), end = lSolidObject.end();
	while (it != end)
	{
		// Delete if done
		delete *it;
		it = lSolidObject.erase(it);
	}
}

/**
 @brief Initialise this class instance
 @return A bool variable
 */
bool CSolidObjectManager::Init(void)
{
	lSolidObject.clear();

	cProjectileManager = CProjectileManager::GetInstance();

	cPlayer3D = CPlayer3D::GetInstance();

	return true;
}

/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void CSolidObjectManager::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CSolidObjectManager::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CSolidObjectManager::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Add a CSolidObject* to this class instance
 @param cSolidObject The CSolidObject* variable to be added to this class instance
 */
void CSolidObjectManager::Add(CSolidObject* cSolidObject)
{
	lSolidObject.push_back(cSolidObject);
}

/**
 @brief Remove a CSolidObject* from this class instance
 @param cSolidObject The CSolidObject* variable to be erased from this class instance
 @return A bool variable
 */
bool CSolidObjectManager::Erase(CSolidObject* cSolidObject)
{
	// Find the entity's iterator
	std::list<CSolidObject*>::iterator findIter = std::find(lSolidObject.begin(), lSolidObject.end(), cSolidObject);

	// Delete the entity if found
	if (findIter != lSolidObject.end())
	{
		// Delete the CSolidObject
		//delete *findIter;
		// Go to the next iteration after erasing from the list
		findIter = lSolidObject.erase(findIter);
		return true;
	}
	// Return false if not found
	return false;
}

/**
 @brief Collision Check for a CSolidObject*
 @param cSolidObject The CSolidObject* variable to be checked
 @return A bool variable
 */
bool CSolidObjectManager::CollisionCheck(CSolidObject* cSolidObject)
{
	std::list<CSolidObject*>::iterator it, end;
	end = lSolidObject.end();
	for (it = lSolidObject.begin(); it != end; ++it)
	{
		// Check for collisions between the 2 entities
		if (CCollisionManager::BoxBoxCollision( cSolidObject->GetPosition() + cSolidObject->boxMin,
												cSolidObject->GetPosition() + cSolidObject->boxMax,
												(*it)->GetPosition() + (*it)->boxMin,
												(*it)->GetPosition() + (*it)->boxMax) == true)
		{
			// Rollback the cSolidObject's position
			cSolidObject->RollbackPosition();
			// Rollback the NPC's position
			(*it)->RollbackPosition();
			if ((*it)->GetType() == CSolidObject::TYPE::NPC) {
				//cout << "** Collision between this Entity and a NPC ***" << endl;

			}
			else if ((*it)->GetType() == CSolidObject::TYPE::OTHERS) {
				//cout << "** Collision between this Entity and an OTHERS ***" << endl;

			}
			else if ((*it)->GetType() == CSolidObject::TYPE::STRUCTURE) {
				//cout << "** Collision between this Entity and a STRUCTURE ***" << endl;

			}
			else if ((*it)->GetType() == CSolidObject::TYPE::PROJECTILE) {
				//cout << "** Collision between this Entity and a PROJECTILE ***" << endl;

			}
			// Quit this loop since a collision has been found
			break;
		}
	}

	return true;
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CSolidObjectManager::Update(const double dElapsedTime)
{
	std::list<CSolidObject*>::iterator it, end;

	// Update all CSolidObject
	end = lSolidObject.end();
	for (it = lSolidObject.begin(); it != end; ++it)
	{
		//set target if needed
		if ((*it)->GetType() == CSolidObject::TYPE::NPC) {
			if ((*it)->TargetEntity == nullptr) {
				TargetEntity((*it));									//selects new target
			}
			else if ((*it)->TargetEntity->GetStatus() == false) {			//if target is dead, clear target
				(*it)->TargetEntity = nullptr;
				TargetEntity((*it));
			}
			else {
				//null
			}
		}

		(*it)->Update(dElapsedTime);
		//std::cout<<"solid object manager update" << std::endl;
	}

	CheckForCollision();

	return true;
}

/**
 @brief Collision Check for all entities and projectiles
 @return A bool variable
 */
bool CSolidObjectManager::CheckForCollision(void)
{
	std::list<CSolidObject*>::iterator it, end;
	std::list<CSolidObject*>::iterator it_other;

	// Check for collisions among entities
	end = lSolidObject.end();
	for (it = lSolidObject.begin(); it != end; ++it)
	{
		// If the entity is not active, then skip it
		if ((*it)->GetStatus() == false)
			continue;

		for (it_other = it; it_other != end; ++it_other)
		{
			// If the entity is not active, then skip it
			if ((*it_other)->GetStatus() == false)
				continue;

			// If the 2 entities to check are the same, then skip this iteration
			if (it_other == it)
				continue;

			// Check for collisions between the 2 entities
			if (CCollisionManager::BoxBoxCollision((*it)->GetPosition() + (*it)->boxMin,
				(*it)->GetPosition() + (*it)->boxMax,
				(*it_other)->GetPosition() + (*it_other)->boxMin,
				(*it_other)->GetPosition() + (*it_other)->boxMax) == true)
			{
				// Check if a movable entity collides with another movable entity
				if (
					((*it)->GetType() == CSolidObject::TYPE::PLAYER)
					&&
					(((*it_other)->GetType() >= CSolidObject::TYPE::NPC) &&
						((*it_other)->GetType() <= CSolidObject::TYPE::OTHERS))
					)
				{
					(*it)->RollbackPosition();
					(*it_other)->RollbackPosition();
					//cout << "** Collision between Player and an Entity ***" << endl;

					if ((*it_other)->GetType() == CSolidObject::TYPE::COLLECTIBLES) {			//add collectible entity here
						//give item
						if ((*it_other)->getID() == 1) {			//health
							cPlayer3D->setHealth(cPlayer3D->getHealth() + (*it_other)->getValue() * cPlayer3D->getMaxHealth());
						}
						else if ((*it_other)->getID() == 2) {			//ammo
							cPlayer3D->GetWeapon()->AddRounds((*it_other)->getValue() * cPlayer3D->GetWeapon()->GetMaxMagRound());
						}
						(*it_other)->SetStatus(false);
					}

					break;
				}
				else if (
						(((*it)->GetType() >= CSolidObject::TYPE::NPC) &&
						((*it)->GetType() <= CSolidObject::TYPE::OTHERS))
						&&
						(((*it_other)->GetType() >= CSolidObject::TYPE::NPC) &&
						((*it_other)->GetType() <= CSolidObject::TYPE::OTHERS))
					)
				{
					(*it)->RollbackPosition();
					(*it_other)->RollbackPosition();
					//cout << "** Collision between 2 Entities ***" << endl;
					break;
				}
				// Check if a movable entity collides with a non-movable entity
				if (
						(((*it)->GetType() >= CSolidObject::TYPE::PLAYER) &&
							((*it)->GetType() <= CSolidObject::TYPE::OTHERS))
						&&
						((*it_other)->GetType() == CSolidObject::TYPE::STRUCTURE)
					)
				{
					(*it)->RollbackPosition();
					(*it)->SetToIdle();
					//cout << "** Collision between Entity and Structure ***" << endl;
					break;
				}
			}
		}
	}

	// Check for collisions between entities and projectiles
	end = lSolidObject.end();
	for (it = lSolidObject.begin(); it != end; ++it) {
		// If the entity is not active, then skip it
		if ((*it)->GetStatus() == false) continue;

		for (unsigned int i = 0; i < cProjectileManager->vProjectile.size(); i++) {
			// If the entity is not active, then skip it
			if ((cProjectileManager->vProjectile[i])->GetStatus() == false)
				continue;

			// Use ray tracing to check for collisions between the 2 entities
			if ((CCollisionManager::RayBoxCollision((*it)->GetPosition() + (*it)->boxMin, (*it)->GetPosition() + (*it)->boxMax,
				(cProjectileManager->vProjectile[i])->GetPreviousPosition(),
				(cProjectileManager->vProjectile[i])->GetPosition()))
				||
				(CCollisionManager::BoxBoxCollision((*it)->GetPosition() + (*it)->boxMin, (*it)->GetPosition() + (*it)->boxMax,
				(cProjectileManager->vProjectile[i])->GetPosition() + (cProjectileManager->vProjectile[i])->boxMin,
				(cProjectileManager->vProjectile[i])->GetPosition() + (cProjectileManager->vProjectile[i])->boxMax))) {
				if ((cProjectileManager->vProjectile[i])->GetSource() == (*it) || 
					(cProjectileManager->vProjectile[i])->GetSource()->getFaction() == (*it)->getFaction())				
					//if the projectile is fired by the same entity or faction, ignore, thus disabling friendly fire
					continue;
				else {
					if ((*it)->GetType() == CSolidObject::TYPE::PLAYER) {
						(*it)->takeDamage((cProjectileManager->vProjectile[i])->GetDamage());
						(cProjectileManager->vProjectile[i])->SetStatus(false);
						//cout << "** RayBoxCollision between Player and Projectile ***" << endl;
						CCameraEffectsManager::GetInstance()->Get("BloodScreen")->SetStatus(true);			//take damage and apply blood screen

						break;
					}
					else if ((*it)->GetType() == CSolidObject::TYPE::NPC) {
						(*it)->takeDamage((cProjectileManager->vProjectile[i])->GetDamage());
						(cProjectileManager->vProjectile[i])->SetStatus(false);
						//cout << "** RayBoxCollision between NPC and Projectile ***" << endl;

						if (cProjectileManager->vProjectile[i]->GetSource() == cPlayer3D) {
							//apply hit marker
						}

						break;
					}
					else if ((*it)->GetType() == CSolidObject::TYPE::STRUCTURE) {
						(cProjectileManager->vProjectile[i])->SetStatus(false);
						//cout << "** RayBoxCollision between Structure and Projectile ***" << endl;
						break;
					}
				}
			}

		}
	}

	return true;
}

/**
 @brief CleanUp all CEntity3Ds which are marked for deletion
 */
void CSolidObjectManager::CleanUp(void)
{
	std::list<CSolidObject*>::iterator it, end;
	it = lSolidObject.begin();
	end = lSolidObject.end();
	while (it != end)
	{
		if ((*it)->IsToDelete())
		{
			// Delete the CSolidObject
			delete *it;
			// Go to the next iteration after erasing from the list
			it = lSolidObject.erase(it);
		}
		else
		{
			// Go to the next iteration
			++it;
		}
	}
}

/**
 @brief Render this class instance
 */
void CSolidObjectManager::Render(void)
{
	// Render all entities
	std::list<CSolidObject*>::iterator it, end;
	end = lSolidObject.end();
	for (it = lSolidObject.begin(); it != end; ++it)
	{
		(*it)->SetView(view);
		(*it)->SetProjection(projection);
		(*it)->PreRender();
		(*it)->Render();
		(*it)->PostRender();
	}
}

bool CSolidObjectManager::TargetEntity(CSolidObject *_self) {		//targets an entity and returns its pointer
	if ((*_self).TargetEntity != nullptr) {							//if there is alr a target, ignore
		return true;
	}

	std::list<CSolidObject*>::iterator it, end;
	end = lSolidObject.end();
	for (it = lSolidObject.begin(); it != end; ++it)				//search through all entities
	{
		if ((*it)->GetStatus() == false)			//if not active, ignore
			continue;
		//checks entity type
		if ((*it)->GetType() == CSolidObject::TYPE::NPC && (*it)->getFaction() != (*_self).getFaction()) { //check faction type
			//targets entity within distance
			if (glm::distance((*_self).GetPosition(), (*it)->GetPosition()) < (*_self).getDetectionDistance()) {
				(*_self).TargetEntity =  *it;
				return true;
			}
		}
	}
	(*_self).TargetEntity = nullptr;
	return false;
}

void CSolidObjectManager::ResetLevel() {
	std::list<CSolidObject*>::iterator it, end;
	it = lSolidObject.begin();
	end = lSolidObject.end();
	while (it != end)
	{
		if ((*it)->GetType() == CSolidObject::TYPE::PLAYER) {
			++it;
			continue;						//dont delete player
		}
		(*it)->SetStatus(false);
		//delete *it;
		//it = lSolidObject.erase(it);
		++it;
	}
	cPlayer3D->SetStatus(true);					//re-enables the player
}
void CSolidObjectManager::SetPlayerPtr(CPlayer3D* _selfPTR) {
	cPlayer3D = _selfPTR;
}
