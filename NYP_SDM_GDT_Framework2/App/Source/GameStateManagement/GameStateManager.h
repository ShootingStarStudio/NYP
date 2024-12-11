#pragma once

#include "DesignPatterns/SingletonTemplate.h"
#include <map>
#include <string>

#include "GameStateBase.h"

class CGameStateManager : public CSingletonTemplate<CGameStateManager>
{
	friend CSingletonTemplate<CGameStateManager>;
public:
	// Destroy this CGameStateManager
	void Destroy(void);

	// Reset the CGameState
	void Reset(void);

	// System Interface
	// Update this class instance
	bool Update(const double dElapsedTime);
	// Render this class instance
	void Render(void);

	// User Interface
	bool AddGameState(const std::string& _name, CGameStateBase* _scene);
	bool RemoveGameState(const std::string& _name);
	bool SetActiveGameState(const std::string& _name);
	bool CheckGameStateExist(const std::string& _name);
	bool SetPauseGameState(const std::string& _name);
	void OffPauseGameState(void);

protected:
	// Constructor
	CGameStateManager(void);
	// Destructor
	~CGameStateManager(void);

	// The map of CGameStates
	std::map<std::string, CGameStateBase*> GameStateMap;
	// The handlers containing the active and next CGameState
	CGameStateBase* activeGameState, *nextGameState, *prevGameState, *pauseGameState;
};
