/**
 CWaypointManager
 By: Toh Da Jun
 Date: Sep 2020
 */
#include "WaypointManager.h"
#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CWaypointManager::CWaypointManager(void)
	: iCurrentID(-1)
	, fDistanceTolerance(16.0f)	// We set the tolerance to 4.0 units squared == 16.0 units
{
	// Initialise this class
	Init();
}

/**
 @brief Default Destructor
 */
CWaypointManager::~CWaypointManager(void)
{
	// Destroy this class
	Destroy();
}

/**
 @brief Initialise this instance to default values
 @return A bool variable to indicate this method successfully completed its tasks
 */
void CWaypointManager::Init(void)
{
	// Set the iCurrentID to zero. 
	iCurrentID = 0;
	// Clear the vWaypoints
	vWaypoints.clear();
}

/**
 @brief Destroy this class instance
 */
void CWaypointManager::Destroy(void)
{
	// If vWaypoints is not empty, then proceed to delete
	if (vWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		// Destroy the children first
		vector<CWaypoint*>::iterator it;
		for (it = vWaypoints.begin(); it != vWaypoints.end(); ++it)
		{
			// Delete an entry
			delete *it;
		}
	}
	// We delete the waypoints here
	vWaypoints.clear();
}

/**
 @brief Add waypoint
 @param position A const glm::vec3 variable containing the position of a waypoint
 @return A int variable
 */
int CWaypointManager::AddWaypoint(const glm::vec3 position)
{
	// Create a new CWayPoint
	CWaypoint* aNewWaypoint = new CWaypoint();
	// Get the next ID
	int m_iNextID = GetNextIDToAdd();
	// Assign to the new WayPoint
	aNewWaypoint->SetID(m_iNextID);
	// Set its position
	aNewWaypoint->SetPosition(position);
	// Push it into the vWaypoints
	vWaypoints.push_back(aNewWaypoint);
	// Return the m_iNextID
	return m_iNextID;
}

/**
 @brief Add a new waypoint to the list by searching for its ID, and it is related to an existing WayPoint
 @param iID A const const int variable containing the ID of a new waypoint
 @param position A const glm::vec3 variable containing the position of a waypoint
 @return A int variable
 */
int CWaypointManager::AddWaypoint(const int iID, const glm::vec3 position)
{
	// If Waypoints is not empty, then we proceed to search.
	if (vWaypoints.size() != 0)
	{
		// Loop through the vWaypoints
		for (unsigned i = 0; i < vWaypoints.size(); ++i)
		{
			CWaypoint* aWayPoint = vWaypoints[i];

			if (aWayPoint->GetID() == iID)
			{
				// Get the next ID
				int m_iNextID = GetNextIDToAdd();
				// Create a new WayPoint
				CWaypoint* aNewWaypoint = new CWaypoint();
				// Set the m_iNextID as the ID for the new waypoint
				aNewWaypoint->SetID(m_iNextID);
				// Set the position of the new waypoint
				aNewWaypoint->SetPosition(position);
				// Add it to vWaypoints
				vWaypoints.push_back(aNewWaypoint);

				// Add the current Waypoint as a related waypoint to the new Waypoint
				aNewWaypoint->AddRelatedWaypoint(aWayPoint);
				// Add this new Waypoint as a related waypoint to the current Waypoint
				aWayPoint->AddRelatedWaypoint(aNewWaypoint);

				return m_iNextID;
			}
		}
	}
	return -1;
}

/**
 @brief Remove waypoint based on its m_iNextIDs
 @param iID A const const int variable containing the ID of a waypoint
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CWaypointManager::RemoveWaypoint(const int iID)
{
	// Declare a boolean flag called result and its default value is false
	bool result = false;

	// If Waypoints is not empty, then we proceed to search.
	if (vWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = vWaypoints.begin();
		while (it != vWaypoints.end())
		{
			// If the WayPoint's ID matches the requested m_iNextID
			if ((*it)->GetID() == iID)
			{
				// Remove all references to this Waypoint from other Waypoints
				RemoveRelatedWaypoint(iID);

				// Remove this Waypoint from the WaypointManager
				delete *it;
				// Erase the waypoint from vWaypoints
				it = vWaypoints.erase(it);
				// Set the boolean result to true as this is successful
				result = true;
				break;
			}

			// Increase the iterator is we have not reach the end of vWaypoints
			if (it != vWaypoints.end())
				it++;
		}
	}
	return result;
}

/**
 @brief Remove related waypoint of a waypoint based on its ID
 @param iID A const const int variable containing the ID of a waypoint
 @return A bool variable to indicate this method successfully completed its tasks
 */
void CWaypointManager::RemoveRelatedWaypoint(const int iID)
{
	// Iterate through all the Waypoints
	std::vector<CWaypoint*>::iterator it = vWaypoints.begin();
	while (it != vWaypoints.end())
	{
		(*it)->RemoveRelatedWaypoint(iID);
		it++;
	}
}

/**
 @brief Get next ID when adding a waypoint
 @return A int variable
 */
int CWaypointManager::GetNextIDToAdd(void)
{
	// The next ID is the size of vWaypoints
	return GetNumberOfWaypoints();
}

/**
 @brief Get ID of the current waypoint
 @return A int variable
 */
int CWaypointManager::GetCurrentID(void)
{
	return iCurrentID;
}

/**
 @brief Get ID of the next waypoint
 @return A int variable
 */
int CWaypointManager::GetNextWayPointID(void)
{
	// If we have reach the end of the vWaypoints, then we reset to 0
	iCurrentID++;
	if (iCurrentID >= (int)vWaypoints.size())
		iCurrentID = 0;

	return iCurrentID;
}

/**
 @brief Check if a position is at a waypoint
 @param position A const glm::vec3 variable containing a position to check
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CWaypointManager::HasReachedWayPoint(const glm::vec3 aPosition)
{
	// Check if there is at least 1 WayPoint
	if (vWaypoints.size() > 0)
	{
		// Get the position of current WayPoint
		glm::vec3 currentWaypointPosition = vWaypoints[iCurrentID]->GetPosition();

		// Calculate the x- and z- component distance
		float xDistance = aPosition.x - currentWaypointPosition.x;
		float zDistance = aPosition.z - currentWaypointPosition.z;
		// Calculate the distance squared value. We avoid square root as it is expensive
		float distanceSquared = (float)(xDistance*xDistance + zDistance*zDistance);
		// if the distance between aPosition and the current WayPoint is within fDistanceTolerance value
		if (distanceSquared < fDistanceTolerance)
		{
			// then return true
			return true;
		}
	}
	// Return false is aPosition is not near the current WayPoint
	return false;
}

/**
 @brief Get a waypoint based on its ID
 @return A CWaypoint* variable
 */
CWaypoint* CWaypointManager::GetWaypoint(const int iID)
{
	// If Waypoints is not empty, then we proceed to search.
	if (vWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = vWaypoints.begin();
		while (it != vWaypoints.end())
		{
			if ((*it)->GetID() == iID)
			{
				// Assign iID to the current ID
				iCurrentID = iID;
				return (CWaypoint*)(*it);
			}
			it++;
		}
	}
	return NULL;
}

/**
 @brief Get nearest waypoint amongst related Waypoints. This method is used when your NPC 
        has deviated from the WayPoints and it needs to find the nearest WayPoint to go to
 @param position A const glm::vec3 variable containing a position to check
 @return A CWaypoint* variable
 */
CWaypoint* CWaypointManager::GetNearestWaypoint(glm::vec3 aPosition)
{
	CWaypoint* theNearestWaypoint = NULL;
	float m_fDistance = numeric_limits<float>::max();

	// If Waypoints has related Waypoints, then we proceed to search.
	if (vWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints to find the nearest WayPoint
		std::vector<CWaypoint*>::iterator it = vWaypoints.begin();
		while (it != vWaypoints.end())
		{
			glm::vec3 aRelatedWaypoint = (*it)->GetPosition();

			float xDistance = aPosition.x - aRelatedWaypoint.x;
			float zDistance = aPosition.z - aRelatedWaypoint.z;
			float distanceSquared = (float)(xDistance*xDistance + zDistance*zDistance);
			if (m_fDistance > distanceSquared)
			{
				// Update the m_fDistance to this lower distance
				m_fDistance = distanceSquared;
				// Set this WayPoint as the nearest WayPoint
				theNearestWaypoint = (CWaypoint*)(*it);
			}
			it++;
		}
	}
	if (theNearestWaypoint)
	{
		// Update the current ID
		iCurrentID = theNearestWaypoint->GetID();
	}
	else
	{
		// Set to a value which is impossible to achieve, so that the program will crash.
		iCurrentID = -1;
	}

	return theNearestWaypoint;
}

/**
 @brief Get next waypoint
 @return A CWaypoint* variable
 */
CWaypoint* CWaypointManager::GetNextWaypoint(void)
{
	// If Waypoints is not empty, then we proceed to search.
	if (vWaypoints.size() != 0)
	{
		iCurrentID = GetNextWayPointID();

		return (CWaypoint*)vWaypoints[iCurrentID];
	}

	return NULL;
}

/**
 @brief Get next waypoint
 @return A glm::vec3 variable
 */
glm::vec3 CWaypointManager::GetNextWaypointPosition(void)
{
	CWaypoint* cWayPoint = GetNextWaypoint();
	if (cWayPoint)
		return cWayPoint->GetPosition();
	else
		// Return the origin is there is no WayPoint available
		return glm::vec3(0.0f, 0.0f, 0.0f);
}

/**
 @brief Get the number of related Waypoints
 */
int CWaypointManager::GetNumberOfWaypoints(void) const
{
	return (int)vWaypoints.size();
}

/**
 @brief PrintSelf
 */
void CWaypointManager::PrintSelf(void)
{
	cout << "================================================================================" << endl;
	cout << "CWaypointManager::PrintSelf" << endl;
	// If Waypoints has related Waypoints, then we proceed to search.
	if (vWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		vector<CWaypoint*>::iterator it = vWaypoints.begin();
		while (it != vWaypoints.end())
		{
			(*it)->PrintSelf();
			it++;
		}
	}
	cout << "================================================================================" << endl;
}
