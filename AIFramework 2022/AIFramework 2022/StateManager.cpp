#include "StateManager.h"

StateManager::StateManager(Vehicle* p_agent)
{
	m_agent = p_agent;
	m_currentState = new State();
	m_previousState = new State();
}

StateManager::~StateManager()
{
	delete m_currentState;
}

void StateManager::Update(float t)
{
	if (m_currentState->completed) {
		ChangeState(m_previousState);
	}
	m_currentState->Update(t);
}

void StateManager::ChangeState(State* newState)
{
	//call the exit function, cleanup old data, switch current state to previous state and enter the new state
	m_currentState->ExitState();
	if (newState == m_previousState) { //if going back to previous state
		m_currentState->Cleanup();
		delete m_currentState;
		m_currentState = newState;
		m_previousState = new State();
		m_currentState->EnterState();
	}
	else {
		m_previousState->Cleanup();
		delete m_previousState;
		m_previousState = m_currentState;
		m_currentState = newState;
		m_currentState->EnterState();
	}

}
