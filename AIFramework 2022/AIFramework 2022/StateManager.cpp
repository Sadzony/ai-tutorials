#include "StateManager.h"

StateManager::StateManager(Vehicle* p_agent)
{
	m_agent = p_agent;
	m_currentState = new State();
}

StateManager::~StateManager()
{
	delete m_currentState;
}

void StateManager::Update(float t)
{
	m_currentState->Update(m_agent, t);
}

void StateManager::ChangeState(State* newState)
{
	m_currentState->ExitState();
	delete m_currentState;
	m_currentState = newState;
	m_currentState->EnterState();
}
