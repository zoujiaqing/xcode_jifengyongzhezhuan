#pragma once

#include <assert.h>

class SimpleStateMachine
{
public:

	enum 
	{
		EVENT_Update,
		EVENT_Enter,
		EVENT_Exit,
        EVENT_User,
	};
    
    typedef int StateMachineEvent;

	SimpleStateMachine( void );
	~SimpleStateMachine( void ) {}

	void Initialize( void );
	void Update( void );
	void Terminate( void );
	void SetState( unsigned int newState );
	void SetCurrentState( unsigned int newState );
	unsigned int GetState(){return m_currentState;}
	bool IsStateChanged() {return m_stateChange;}
    void Process( StateMachineEvent event );

	bool IsRunning(){return m_bInitialized;}

private:

	unsigned int				m_currentState;
	unsigned int				m_nextState;
	bool						m_stateChange;
	bool						m_bInitialized;
	

protected:
	virtual bool States( StateMachineEvent event, int state ) = 0;

};

