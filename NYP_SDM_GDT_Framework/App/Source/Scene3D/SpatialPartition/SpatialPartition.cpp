#include "SpatialPartition.h"

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include Terrain
#include "../Terrain/TerrainManager.h"
// Include Player3D
#include "../Entities/Player3D.h"

// Include this for glm::to_string() function
#define GLM_ENABLE_EXPERIMENTAL
#include <includes/gtx/string_cast.hpp>

#include <iostream>
using namespace std;

/**
@brief Constructor
*/
CSpatialPartition::CSpatialPartition(void)
	: vec3SpatialPartitionSize(glm::vec3(1.0f))
	, vec3GridSize(glm::vec3(1.0f))
	, i32vec3NumGrid(glm::i32vec3(1))
	, vec3TopRight(glm::vec3(1.0f))
	, vec3BottomLeft(glm::vec3(1.0f))
	, vec4Colour(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f))
	, fLineWidth(10.0f)
	, fPreviousLineWidth(0.0f)
	, pFrustumCulling(NULL)
	, frustumCullingView(glm::mat4(1.0f))
{
}

/**
@brief Destructor
*/
CSpatialPartition::~CSpatialPartition(void)
{

}

/**
@brief DeInitialise this instance
*/
void CSpatialPartition::DeInit()
{
	if (pFrustumCulling)
	{
		delete pFrustumCulling;
		pFrustumCulling = NULL;
	}

	// Destroy all the CGrids
	for (int iRow = 0; iRow < i32vec3NumGrid.x; iRow++)
	{
		for (int iCol = 0; iCol < i32vec3NumGrid.z; iCol++)
		{
			delete vGrids[iRow][iCol];
		}
	}

	CSingletonTemplate<CSpatialPartition>::Destroy();
}

/**
@brief Initialise the spatial partition
@param vec3SpatialPartitionSize A glm::vec3 variable storing the size of the spatial partition
@param i32vec3NumGrid A glm::i32vec3 variable storing the number of grids in the spatial partition
*/
bool CSpatialPartition::Init(void)
{
	return Init(glm::vec3(100.0f, 1.0f, 100.0f), glm::i32vec3(10, 1, 10));
}

/**
@brief Initialise the spatial partition
@param vec3SpatialPartitionSize A glm::vec3 variable storing the size of the spatial partition
@param i32vec3NumGrid A glm::i32vec3 variable storing the number of grids in the spatial partition
*/
bool CSpatialPartition::Init(glm::vec3 vec3SpatialPartitionSize, glm::i32vec3 i32vec3NumGrid)
{
	this->vec3SpatialPartitionSize = vec3SpatialPartitionSize;
	this->i32vec3NumGrid = i32vec3NumGrid;

	// Update the size of each tile in X-, Z-axes
	vec3GridSize = vec3SpatialPartitionSize / glm::vec3(i32vec3NumGrid);

	// The coordinate of the rear-left corner of the map in X-, Z-axes
	vec3BottomLeft = -(vec3SpatialPartitionSize / 2.0f);
	vec3BottomLeft.y = 0.0f;
	// The coordinate of the front-right corner of the map in X-, Z-axes
	vec3TopRight = (vec3SpatialPartitionSize / 2.0f);
	vec3TopRight.y = 0.0f;

	CGrid* cGrid = NULL;
	// Update the vGrid* vector of vectors
	for (int iRow = 0; iRow < i32vec3NumGrid.x; iRow++)
	{
		// Construct a vector of int
		vector<CGrid*> vColumnsOfValues;
		for (int iCol = 0; iCol < i32vec3NumGrid.z; iCol++)
		{
			// Create a new CGrid
			cGrid = new CGrid();
			// Calculate the grid's bottom left coordinates
			glm::vec3 vec3GridBottomLeft = glm::vec3(vec3BottomLeft.x + vec3GridSize.x * (float)iRow,
													 0.0f,
													 vec3BottomLeft.z + vec3GridSize.z * (float)iCol);
			// Calculate the grid's bottom left y-coordinates
			vec3GridBottomLeft.y = CTerrainManager::GetInstance()->GetHeight(vec3BottomLeft.x + vec3GridSize.x * (float)iRow,
																			vec3BottomLeft.z + vec3GridSize.z * (float)iCol);
			// Calculate the grid's top right coordinates
			glm::vec3 vec3GridTopRight = vec3GridBottomLeft + vec3GridSize;
			// Calculate the grid's top right y-coordinates
			vec3GridTopRight.y = CTerrainManager::GetInstance()->GetHeight(vec3GridTopRight.x, vec3GridTopRight.z);

			// Set the shader to this grid
			cGrid->SetShader(sShaderName);
			// Initialise the grid
			cGrid->Init(vec3GridTopRight, vec3GridBottomLeft,
						CTerrainManager::GetInstance()->GetHeight(vec3GridBottomLeft.x, vec3GridTopRight.z),	// fTopLeftHeight,
						CTerrainManager::GetInstance()->GetHeight(vec3GridTopRight.x, vec3GridBottomLeft.z)	// fBottomRightHeight)
						);

			// Add the ground tile into the vColumnsOfValues vector
			vColumnsOfValues.push_back(cGrid);
		}
		// Add the vColumnsOfValues vector into the vGroundTiles vector
		vGrids.push_back(vColumnsOfValues);
	}

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		vec3BottomLeft.x, vec3BottomLeft.y, vec3BottomLeft.z, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		vec3TopRight.x, vec3BottomLeft.y, vec3BottomLeft.z, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		vec3TopRight.x, vec3BottomLeft.y, vec3TopRight.z, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		vec3TopRight.x, vec3BottomLeft.y, vec3TopRight.z, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		vec3BottomLeft.x, vec3BottomLeft.y, vec3TopRight.z, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		vec3BottomLeft.x, vec3BottomLeft.y, vec3BottomLeft.z, vec4Colour.x, vec4Colour.y, vec4Colour.z,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	pFrustumCulling = new CFrustumCulling();
	pFrustumCulling->SetShader("Shader3D_Line");

	//frustumCullingView = CPlayer3D::GetInstance()->GetViewMatrix();
	pFrustumCulling->SetModel(CPlayer3D::GetInstance()->GetModel());
	pFrustumCulling->SetView(view);
	pFrustumCulling->SetProjection(projection);
	pFrustumCulling->Init();

	return false;
}

/**
@brief Update this class instance
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
*/
bool CSpatialPartition::Update(const double dElapsedTime)
{
	frustumCullingView = CPlayer3D::GetInstance()->GetViewMatrix();

	pFrustumCulling->SetView(frustumCullingView);
	pFrustumCulling->SetProjection(CPlayer3D::GetInstance()->GetPerspectiveMatrix());
	pFrustumCulling->Update(dElapsedTime);

	// Determine the visibility of all the CGrids
	for (int iRow = 0; iRow < i32vec3NumGrid.x; iRow++)
	{
		for (int iCol = 0; iCol < i32vec3NumGrid.z; iCol++)
		{
			glm::vec3 vec3BottomRight = glm::vec3(vGrids[iRow][iCol]->vec3TopRight.x, vGrids[iRow][iCol]->vec3BottomLeft.y, vGrids[iRow][iCol]->vec3BottomLeft.z);
			glm::vec3 vec3TopLeft = glm::vec3(vGrids[iRow][iCol]->vec3BottomLeft.x, vGrids[iRow][iCol]->vec3TopRight.y, vGrids[iRow][iCol]->vec3TopRight.z);
				 
			//	vGrids[iRow][iCol]->vec3TopRight) == true)
			if ((pFrustumCulling->IsPointWithin(vGrids[iRow][iCol]->vec3BottomLeft) == true) ||
				(pFrustumCulling->IsPointWithin(vGrids[iRow][iCol]->vec3TopRight) == true) ||
				(pFrustumCulling->IsPointWithin(vec3BottomRight) == true) ||
				(pFrustumCulling->IsPointWithin(vec3TopLeft) == true))
			{
				// Set the grid to visible
				vGrids[iRow][iCol]->SetVisiblity(true);

				// Check the distance of the grid to the player and determine the LOD
				float fDistance = glm::distance(glm::vec3(frustumCullingView[3]), vGrids[iRow][iCol]->GetPosition());
				vGrids[iRow][iCol]->UpdateLOD(fDistance);

				// Update this grid
				vGrids[iRow][iCol]->Update(dElapsedTime);
			}
			else
			{
				// Set the grid to not visible
				vGrids[iRow][iCol]->SetVisiblity(false);
			}
		}
	}

	// Set the grid which the player is in to visible by default
	CGrid* tempGrid = GetGrid(CPlayer3D::GetInstance()->GetPosition() - glm::vec3(CPlayer3D::GetInstance()->vec3BoxMin.x, 0.0f, CPlayer3D::GetInstance()->vec3BoxMin.z));
	if (tempGrid)
		tempGrid->SetVisiblity(true);
	tempGrid = GetGrid(CPlayer3D::GetInstance()->GetPosition() - glm::vec3(CPlayer3D::GetInstance()->vec3BoxMin.x, 0.0f, CPlayer3D::GetInstance()->vec3BoxMax.z));
	if (tempGrid)
		tempGrid->SetVisiblity(true);
	tempGrid = GetGrid(CPlayer3D::GetInstance()->GetPosition() - glm::vec3(CPlayer3D::GetInstance()->vec3BoxMax.x, 0.0f, CPlayer3D::GetInstance()->vec3BoxMin.z));
	if (tempGrid)
		tempGrid->SetVisiblity(true);
	tempGrid = GetGrid(CPlayer3D::GetInstance()->GetPosition() - glm::vec3(CPlayer3D::GetInstance()->vec3BoxMax.x, 0.0f, CPlayer3D::GetInstance()->vec3BoxMax.z));
	if (tempGrid)
		tempGrid->SetVisiblity(true);

	return true;
}

/**
@brief PreRender
*/
void CSpatialPartition::PreRender(void)
{
	// change depth function so depth test passes when values are equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);

	// Activate shader
	CShaderManager::GetInstance()->Use(sShaderName);

	glGetFloatv(GL_LINE_WIDTH, &fPreviousLineWidth);
	glLineWidth(fLineWidth);
}

/**
@brief Render
*/
void CSpatialPartition::Render(void)
{
	CShaderManager::GetInstance()->pActiveShader->setMat4("model", model);
	CShaderManager::GetInstance()->pActiveShader->setMat4("view", view);
	CShaderManager::GetInstance()->pActiveShader->setMat4("projection", projection);

	// render box
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_LOOP, 0, 6);
	glBindVertexArray(0);

	// Render all the CGrids
	for (int iRow = 0; iRow < i32vec3NumGrid.x; iRow++)
	{
		for (int iCol = 0; iCol < i32vec3NumGrid.z; iCol++)
		{
			if (vGrids[iRow][iCol]->GetVisibility() == true)
			{
				vGrids[iRow][iCol]->SetView(view);
				vGrids[iRow][iCol]->SetProjection(projection);
				vGrids[iRow][iCol]->PreRender();
				vGrids[iRow][iCol]->Render();
				vGrids[iRow][iCol]->PostRender();
			}
		}
	}

	// Render the Frustum
	if (!CPlayer3D::GetInstance()->IsCameraAttached())
	{
		//pFrustumCulling->SetModel(CPlayer3D::GetInstance()->GetModel());
		pFrustumCulling->SetView(view);
		pFrustumCulling->SetProjection(projection);
		pFrustumCulling->PreRender();
		pFrustumCulling->Render();
		pFrustumCulling->PostRender();
	}
}

/**
@brief PostRender
*/
void CSpatialPartition::PostRender(void)
{
	glLineWidth(fPreviousLineWidth);
}

/**
@brief Get a particular grid
@param iRow A const int variable storing the row (x-axis) index of the grid
@param iCol A const int variable storing the column (y-axis) index of the grid
*/
CGrid* CSpatialPartition::GetGrid(const int iRow, const int iCol) const
{
	if (((iRow >= 0) && (iRow < i32vec3NumGrid.x)) && ((iCol >= 0) && (iCol < i32vec3NumGrid.z)))
		return vGrids[iRow][iCol];
	else
		return NULL;
}

/**
@brief Get a particular grid using a position
@param iRow A const int variable storing the row (x-axis) index of the grid
@param iCol A const int variable storing the column (y-axis) index of the grid
*/
CGrid* CSpatialPartition::GetGrid(glm::vec3 vecPosition) const
{
	// Get the indices of the object's position
	int iRow = (int)((vecPosition.x + (vec3SpatialPartitionSize.x * 0.5f)) / (vec3SpatialPartitionSize.x / i32vec3NumGrid.x));
	int iCol = (int)((vecPosition.z + (vec3SpatialPartitionSize.z * 0.5f)) / (vec3SpatialPartitionSize.z / i32vec3NumGrid.z));

	// Add them to each grid
	if (((iRow >= 0) && (iRow < i32vec3NumGrid.x)) && ((iCol >= 0) && (iCol < i32vec3NumGrid.z)))
	{
		return vGrids[iRow][iCol];
	}

	return NULL;
}

/**
@brief Reset the list of CSolidObjects in the Spatial Partition
@param pSolidObjectManager A CSolidObjectManager* handler containing all CSolidObjects in the scene
*/
void CSpatialPartition::Reset(CSolidObjectManager* pSolidObjectManager)
{
	// Determine the visibility of all the CGrids
	for (int iRow = 0; iRow < i32vec3NumGrid.x; iRow++)
	{
		for (int iCol = 0; iCol < i32vec3NumGrid.z; iCol++)
		{
			vGrids[iRow][iCol]->DeInit();
		}
	}

	if (pSolidObjectManager == NULL)
		return;

	// Add the CSolidObjects into the Spatial Partition
	std::list<CSolidObject*>::iterator it, end;

	// Update all CSolidObject
	end = pSolidObjectManager->lSolidObject.end();
	for (it = pSolidObjectManager->lSolidObject.begin(); it != end; ++it)
	{
		//(*it)->Update(dElapsedTime);
		this->Add(*it);
	}

}

/**
@brief Add a new object model
@param cSolidObject A CSolidObject* variable storing the handler of the Scene Node to add
*/
void CSpatialPartition::Add(CSolidObject* cSolidObject)
{
		// Get the indices of the object's vec3BoxMin position
		int iRow = (int)((cSolidObject->GetPosition().x - cSolidObject->vec3BoxMin.x + (vec3SpatialPartitionSize.x * 0.5f)) / (vec3SpatialPartitionSize.x / i32vec3NumGrid.x));
		int iCol = (int)((cSolidObject->GetPosition().z - cSolidObject->vec3BoxMin.z + (vec3SpatialPartitionSize.z * 0.5f)) / (vec3SpatialPartitionSize.z / i32vec3NumGrid.z));
		// Add this position into each grid
		if (((iRow >= 0) && (iRow < i32vec3NumGrid.x)) && ((iCol >= 0) && (iCol < i32vec3NumGrid.z)))
		{
			//cout << "Add to [" << iRow << "][" << iCol << "]" << endl;
			vGrids[iRow][iCol]->Add(cSolidObject);
		}

		// Get the indices of the object's vec3BoxMin position
		iRow = (int)((cSolidObject->GetPosition().x + cSolidObject->vec3BoxMax.x + (vec3SpatialPartitionSize.x * 0.5f)) / (vec3SpatialPartitionSize.x / i32vec3NumGrid.x));
		iCol = (int)((cSolidObject->GetPosition().z + cSolidObject->vec3BoxMin.z + (vec3SpatialPartitionSize.z * 0.5f)) / (vec3SpatialPartitionSize.z / i32vec3NumGrid.z));
		// Add this position into each grid
		if (((iRow >= 0) && (iRow < i32vec3NumGrid.x)) && ((iCol >= 0) && (iCol < i32vec3NumGrid.z)))
		{
			//cout << "Add to [" << iRow << "][" << iCol << "]" << endl;
			vGrids[iRow][iCol]->Add(cSolidObject);
		}

		// Get the indices of the object's vec3BoxMin position
		iRow = (int)((cSolidObject->GetPosition().x - cSolidObject->vec3BoxMin.x + (vec3SpatialPartitionSize.x * 0.5f)) / (vec3SpatialPartitionSize.x / i32vec3NumGrid.x));
		iCol = (int)((cSolidObject->GetPosition().z + cSolidObject->vec3BoxMax.z + (vec3SpatialPartitionSize.z * 0.5f)) / (vec3SpatialPartitionSize.z / i32vec3NumGrid.z));
		// Add this position into each grid
		if (((iRow >= 0) && (iRow < i32vec3NumGrid.x)) && ((iCol >= 0) && (iCol < i32vec3NumGrid.z)))
		{
			//cout << "Add to [" << iRow << "][" << iCol << "]" << endl;
			vGrids[iRow][iCol]->Add(cSolidObject);
		}

		// Get the indices of the object's vec3BoxMin position
		iRow = (int)((cSolidObject->GetPosition().x + cSolidObject->vec3BoxMax.x + (vec3SpatialPartitionSize.x * 0.5f)) / (vec3SpatialPartitionSize.x / i32vec3NumGrid.x));
		iCol = (int)((cSolidObject->GetPosition().z + cSolidObject->vec3BoxMax.z + (vec3SpatialPartitionSize.z * 0.5f)) / (vec3SpatialPartitionSize.z / i32vec3NumGrid.z));
		// Add this position into each grid
		if (((iRow >= 0) && (iRow < i32vec3NumGrid.x)) && ((iCol >= 0) && (iCol < i32vec3NumGrid.z)))
		{
			//cout << "Add to [" << iRow << "][" << iCol << "]" << endl;
			vGrids[iRow][iCol]->Add(cSolidObject);
		}
}

/**
@brief Remove but not delete object from this grid
@param cSolidObject A CSolidObject* variable storing the handler of the Scene Node to remove
*/
void CSpatialPartition::Remove(CSolidObject* cSolidObject)
{
	// Get the indices of the object's position
	//int iRow = (((int)cSolidObject->localTransform[3][0] - (-((int)vec3SpatialPartitionSize.x) >> 1)) / (vec3SpatialPartitionSize.x / i32vec3NumGrid.x));
	//int iCol = (((int)cSolidObject->localTransform[3][2] - (-((int)vec3SpatialPartitionSize.z) >> 1)) / (vec3SpatialPartitionSize.z / i32vec3NumGrid.z));
	int iRow = int(((int)cSolidObject->GetPosition().x - (-((int)vec3SpatialPartitionSize.x) >> 1)) / (vec3SpatialPartitionSize.x / i32vec3NumGrid.x));
	int iCol = int(((int)cSolidObject->GetPosition().z - (-((int)vec3SpatialPartitionSize.z) >> 1)) / (vec3SpatialPartitionSize.z / i32vec3NumGrid.z));

	// Remove from the grid
	if (((iRow >= 0) && (iRow < i32vec3NumGrid.x)) && ((iCol >= 0) && (iCol < i32vec3NumGrid.z)))
	{
		vGrids[iRow][iCol]->Remove(cSolidObject);
	}
}

/**
@brief Collision Check for an entity against Spatial Partition's entities
*/
bool CSpatialPartition::CheckForCollisionWithEntity(CEntity3D* cEntity3D)
{
	bool bResult = false;

	//if (cEntity3D == NULL)
	//	return false;

	//// Get the grid which the cEntity3D is in
	//CGrid* cGrid = GetGrid(cEntity3D->GetPosition());

	//// If it is NULL, then return false
	//if (cGrid == NULL)
	//	return false;

	//// Get the list of scene nodes stored in this grid
	//float m_fMinSphereSphereDistance = 0.0f;
	//for (int i = 0; i < cGrid->ListOfObjects.size(); i++)
	//{
	//	// Calculate the distance between the 2 entities' spheres
	//	m_fMinSphereSphereDistance = cGrid->ListOfObjects[i]->cEntity3D->GetSphereRadius() +
	//									cEntity3D->GetSphereRadius();
	//	// Check for sphere-sphere collision between the 2 entities
	//	if (glm::distance(cGrid->ListOfObjects[i]->cEntity3D->GetPosition(), cEntity3D->GetPosition())
	//		< m_fMinSphereSphereDistance)
	//	{
	//		// Rollback the player's position
	//		cEntity3D->RollbackPosition();
	//		// Set bResult to true
	//		bResult = true;
	//		// Quit this loop since a collision has been found
	//		break;
	//	}
	//}

	return bResult;
}

/**
@brief PrintSelf
*/
void CSpatialPartition::PrintSelf(void) const
{
	cout << "******* Start of CSpatialPartition::PrintSelf() **********************************" << endl;
	cout << "Size\t:\t[" << vec3SpatialPartitionSize.x << ", " << vec3SpatialPartitionSize.z << "]" << endl;
	cout << "NumOfGrid\t:\t[" << i32vec3NumGrid.x << ", " << i32vec3NumGrid.z << "]" << endl;
	cout << "Printing out theGrid below: " << endl;
	for (int iRow = 0; iRow < i32vec3NumGrid.x; iRow++)
	{
		cout << iRow << ": ";
		for (int iCol = 0; iCol < i32vec3NumGrid.z; iCol++)
		{
			if (vGrids[iRow][iCol]->GetVisibility() == true)
				cout << iCol << "(Y/" << vGrids[iRow][iCol]->GetNumOfObject() << "), ";
			else
				cout << iCol << "(N/" << vGrids[iRow][iCol]->GetNumOfObject() << "), ";
		}
		cout << endl;
	}

	cout << "Printing out the frustum view" << endl;
	pFrustumCulling->PrintSelf();

	cout << "******* End of CSpatialPartition::PrintSelf() **********************************" << endl;
}
