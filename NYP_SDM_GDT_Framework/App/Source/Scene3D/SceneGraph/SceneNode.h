/**
 CSceneNode
 By: Toh Da Jun
 Date: Sep 2020
 */
#pragma once

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include <vector>
using namespace std;

class CSceneNode : public CEntity3D
{
public:
	// Constructor
	CSceneNode(void);
	// Constructor
	CSceneNode(const int iID);
	// Destructor
	virtual ~CSceneNode(void);

	// Initialise this class instance
	bool Init(void);

	// DeInitialise this instance
	virtual void DeInit(void);

	// Set model
	virtual void SetModel(const glm::mat4 model);
	// Set view
	virtual void SetView(const glm::mat4 view);
	// Set projection
	virtual void SetProjection(const glm::mat4 projection);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	// Add a child to this CSceneNode
	virtual bool AddChild(CSceneNode* s = NULL);

	// Delete a child from this node using the pointer to the CSceneNode
	virtual bool DeleteChild(CSceneNode* s = NULL);
	// Delete a child from this node using its iID
	virtual bool DeleteChild(const int iID);
	// Delete all children from this node using its iID
	virtual bool DeleteAllChildren(void);

	// Detach a child from this node using its iID
	virtual CSceneNode* DetachChild(const int iID);

	// Get a child from this node using its iID
	virtual CSceneNode* GetChild(const int iID);

	// Return the number of children in this group
	virtual int GetNumOfChild(void);

	// Set the world translate transformation matrix
	void SetWorldTranslateMtx(const glm::vec3 vec3Translate);
	// Set the world rotation transformation matrix
	void SetWorldRotateMtx(const float fAngle, glm::vec3 vec3RotateAxis);
	// Set the world scale transformation matrix
	void SetWorldScaleMtx(const glm::vec3 vec3Scale);

	// Set the local translate transformation matrix
	void SetLocalTranslateMtx(const glm::vec3 vec3Translate);
	// Set the local rotation transformation matrix
	void SetLocalRotateMtx(const float fAngle, const glm::vec3 vec3RotateAxis);
	// Set the local scale transformation matrix
	void SetLocalScaleMtx(const glm::vec3 vec3Scale);

	// Set the update transformation for translation for every update
	void SetUpdateTranslateMtx(const glm::vec3 vec3Translate);
	// Set the update transformation for rotation for every update
	void SetUpdateRotateMtx(const float fAngle, const glm::vec3 vec3RotateAxis);
	// Set the update transformation for scale for every update
	void SetUpdateScaleMtx(const glm::vec3 vec3Scale);

	// PrintSelf for debug purposes
	virtual void PrintSelf(const int numTabs = 0);

	// The iID of this CSceneNode. Easier for identifying the node.
	int	iID;

	// The handler to the parent node. NULL if this CSceneNode is the parent.
	CSceneNode* pParentNode;

	// The vector containing the child nodes
	vector<CSceneNode*> vChildNodes;

	// Boolean flag to indicate if the updates to the localTransformMtx is continuous
	bool bContinuousUpdate;

protected:
	// The world transformation: the transformation from the origin to this child node's location
	glm::mat4 worldTransformMtx;

	// The world translate transformation matrix
	glm::mat4 worldTranslateMtx;
	// The world rotation transformation matrix
	glm::mat4 worldRotateMtx;
	// The world scale transformation matrix
	glm::mat4 worldScaleMtx;

	// The local transformation: the transformation from the parent to this child node's location
	glm::mat4 localTransformMtx;
	// The local transformation: the transformation from the parent to this child node's location WITHOUT SCALE
	glm::mat4 localTransformWOSMtx;

	// The local translate transformation matrix
	glm::mat4 localTranslateMtx;
	// The local rotation transformation matrix
	glm::mat4 localRotateMtx;
	// The local scale transformation matrix
	glm::mat4 localScaleMtx;

	// The update transformation: the update to the translation for every update
	glm::mat4	updateTranslateMtx;
	// The update transformation: the update to the rotation for every update
	glm::mat4	updateRotateMtx;
	// The update transformation: the update to the scale for every update
	glm::mat4	updateScaleMtx;

	unsigned int specularMap;
};
