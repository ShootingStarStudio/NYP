/**
 CWaypoint
 By: Toh Da Jun
 Date: Sep 2020
 */
#include "Waypoint.h"
#include <limits>
#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CWaypoint::CWaypoint(void)
	: vec3Position(glm::vec3(0.0f, 0.0f, 0.0f))
	, iID(-1)
{
	// Init this class instance
	Init();
}

/**
 @brief Default Destructor
 */
CWaypoint::~CWaypoint(void)
{
	// Destroy this class instance
	Destroy();
}

/**
 @brief Initialise this instance to default values
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CWaypoint::Init(void)
{
	// Clear the vRelatedWaypoints
	vRelatedWaypoints.clear();

	return true;
}

/**
 @brief Destroy this class instance
 */
void CWaypoint::Destroy(void)
{
	// Clear the vRelatedWaypoints
	vRelatedWaypoints.clear();
}

/**
 @brief Set position of this waypoint
 @param position A const glm::vec3 variable containing the new position
 */
void CWaypoint::SetPosition(glm::vec3 vec3Position)
{
	this->vec3Position = vec3Position;
}
/**
 @brief Get position of this waypoint
 @return A const glm::vec3 variable
 */
glm::vec3 CWaypoint::GetPosition(void) const
{
	return vec3Position;
}

/**
 @brief Set an ID to this WayPoint
 @param iID A const int variable containing the ID of a new WayPoint
 */
void CWaypoint::SetID(const int iID)
{
	this->iID = iID;
}
/**
 @brief Get an ID of this WayPoint
 @return A const int variable containing the ID of this WayPoint
 */
int CWaypoint::GetID(void) const
{
	return iID;
}

/**
 @brief Add waypoint related to this WayPoint
 @param relatedWaypoint A CWayPoint variable of the related WayPoint
 */
void CWaypoint::AddRelatedWaypoint(CWaypoint* relatedWaypoint)
{
	vRelatedWaypoints.push_back(relatedWaypoint);
}

/**
 @brief Use a waypoint's ID to find and remove a WayPoint related to this WayPoint
 @param A const int variable containing the ID of this WayPoint
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CWaypoint::RemoveRelatedWaypoint(const int iID)
{
	// If Waypoints has related Waypoints, then we proceed to search.
	if (vRelatedWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = vRelatedWaypoints.begin();
		while (it != vRelatedWaypoints.end())
		{
			if ((*it)->GetID() == iID)
			{
				// We erase from the vector, vRelatedWaypoints, only. DO NOT DELETE!
				// Let CWayPointManager delete it!
				it = vRelatedWaypoints.erase(it);
				return true;
			}
			it++;
		}
	}
	return false;
}

/**
 @brief Get nearest Waypoint amongst Waypoints related to this WayPoint
 @return A CWaypoint variable
 */
CWaypoint* CWaypoint::GetNearestWaypoint(void)
{
	CWaypoint* theNearestWaypoint = NULL;
	float m_fDistance = numeric_limits<float>::max();

	// If Waypoints has related Waypoints, then we proceed to search.
	if (vRelatedWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = vRelatedWaypoints.begin();
		while (it != vRelatedWaypoints.end())
		{
			// Get position of a WayPoint
			glm::vec3 aRelatedWaypoint = (*it)->GetPosition();

			// Get the x- and z-component distance between position and aRelatedWaypoint
			float xDistance = vec3Position.x - aRelatedWaypoint.x;
			float zDistance = vec3Position.x - aRelatedWaypoint.z;
			// Calculate the distance squared between position and aRelatedWaypoint
			float distanceSquared = (float)(xDistance*xDistance + zDistance*zDistance);
			if (m_fDistance > distanceSquared)
			{
				// Set distanceSquared to m_fDistance
				m_fDistance = distanceSquared;
				// Set theNearestWaypoint to this WayPoint
				theNearestWaypoint = (CWaypoint*)(*it);
			}

			// Increment the iterator
			it++;
		}
	}
	// Return the nearest WayPoint
	return theNearestWaypoint;
}

/**
 @brief Get the number of related Waypoints
 @return An int variable
 */
int CWaypoint::GetNumberOfWaypoints(void) const
{
	return vRelatedWaypoints.size();
}

/**
 @brief PrintSelf
 */
void CWaypoint::PrintSelf(void)
{
	cout << "Waypoint ID	: " << iID << endl;
	cout << " Position		: " << vec3Position.x << ", " << vec3Position.y << ", " << vec3Position.z << endl;
	cout << " Related waypoints : ";
	// If Waypoints has related Waypoints, then we proceed to search.
	if (vRelatedWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		vector<CWaypoint*>::iterator it = vRelatedWaypoints.begin();
		while (it != vRelatedWaypoints.end())
		{
			cout << (*it)->GetID();
			it++;

			if (it != vRelatedWaypoints.end())
				cout << ", ";
		}
		cout << endl;
	}
	else
		cout << "Nil" << endl;
}
