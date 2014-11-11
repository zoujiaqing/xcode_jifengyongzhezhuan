#include "simplefsm.h"

SimpleStateMachine::SimpleStateMachine( void )
{
	m_bInitialized = false;
	m_currentState = 0;
	m_stateChange = false;
	m_nextState = false;
}

void SimpleStateMachine::Initialize( void )
{
	if (m_bInitialized)
		return;

	m_bInitialized = true;
	Process( EVENT_Enter );
}


void SimpleStateMachine::Update( void )
{
	if (!m_bInitialized)
		return;
	assert(m_bInitialized&&"Update: The state machine has not been initialized");	
	Process( EVENT_Update );
}

void SimpleStateMachine::Terminate( void )
{
	if (!m_bInitialized)
		return;

	Process( EVENT_Exit );
	m_bInitialized = false;
}

void SimpleStateMachine::Process( StateMachineEvent event )
{
	States( event, m_currentState );

	// Check for a state change
	int safetyCount = 10;
	while( m_stateChange && (--safetyCount >= 0) )
	{
		assert( safetyCount > 0 && "SimpleStateMachine::Process - States are flip-flopping in an infinite loop." );

		m_stateChange = false;

		// Let the last state clean-up
		States( EVENT_Exit, m_currentState );

		// Set the new state
		m_currentState = m_nextState;

		// Let the new state initialize
		States( EVENT_Enter, m_currentState );
	}
}


void SimpleStateMachine::SetState( unsigned int newState )
{
	if (m_currentState == newState)
		return;

	m_stateChange = true;
	m_nextState = newState;
}

void SimpleStateMachine::SetCurrentState( unsigned int newState )
{
	m_currentState = newState;
}



