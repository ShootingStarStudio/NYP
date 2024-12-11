/**
 CWaypoint
 By: Toh Da Jun
 Date: Sep 2020
 */
#pragma once

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include <vector>
using namespace std;

class CWaypoint
{
public:
	// Constructor
	CWaypoint();
	// Destructor
	virtual ~CWaypoint();

	// Init this class instance
	bool Init(void);
	// Destroy this class instance
	void Destroy(void);

	// Set position
	void SetPosition(glm::vec3 position);
	// Get position
	glm::vec3 GetPosition(void) const;

	// Set ID
	void SetID(const int iID);
	// Get position
	int GetID(void) const;

	// Add related waypoint
	void AddRelatedWaypoint(CWaypoint* relatedWaypoint);
	// Remove related waypoint
	bool RemoveRelatedWaypoint(const int iID);
	// Get nearest waypoint amongst related Waypoints
	CWaypoint* GetNearestWaypoint(void);
	// Get the number of related Waypoints
	int GetNumberOfWaypoints(void) const;

	// PrintSelf
	void PrintSelf(void);

protected:
	// Position of this WayPoint
	glm::vec3 vec3Position;

	// The ID of this WayPoint
	int iID;

	// Vector of related WayPoints
	vector<CWaypoint*> vRelatedWaypoints;
};