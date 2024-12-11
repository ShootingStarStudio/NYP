/**
 CGameStateBase
 @brief This is an Abstract Class. Derive from this class to implement it.
 By: Toh Da Jun
 Date: July 2021
 */

#pragma once

#include <string>
using namespace std;

class CGameStateBase
{
public:
	// Constructor
	CGameStateBase(void) {}
	// Destructor
	virtual ~CGameStateBase(void) {}

	// These are pure virtual functions
	virtual bool Init() = 0;
	virtual bool Update(const double dElapsedTime) = 0;
	virtual void Render() = 0;
	virtual void Destroy() = 0;

	// Set the name of this game state
	void SetName(const string sName)
	{
		this->sName = sName;
	}
	// Get the name of this game state
	string GetName(void) const
	{
		return sName;
	}

protected:
	// Name of this game state
	string sName;
};
