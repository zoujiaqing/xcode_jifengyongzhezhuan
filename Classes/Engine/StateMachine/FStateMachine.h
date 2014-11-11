// TAOMEE GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Taomee Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 2012-2015 Taomee Game Technologies. 
//      All Rights Reserved.
//
// Taomee Game Technologies, Shanghai, China
// http://www.taomee.com
//
#pragma once

#include <map>
#include <list>
#include <vector>
#include <algorithm>

#include "StateEventType.h"
#include "cocos2d.h"

using namespace std;
USING_NS_CC;

class FEvent;
class FAbstractState;
class FState;
class GroupState;
class FStateMachine;



/// <summary>
/// The FEvent class.
/// </summary>
class FEvent
{
public:
	FState* m_Sender;

	FEvent()
	{
		m_EvtType = ET_None;
		m_Sender = NULL;
	}

	/// <summary>
	/// Gets the event type.
	/// </summary>
	EventType GetEventType()
	{
		return m_EvtType;
	}

	virtual FEvent* Clone()
	{
		FEvent* pCloneEvent = new FEvent();
		pCloneEvent->m_Sender = this->m_Sender;
		pCloneEvent->m_EvtType = this->m_EvtType;
		return pCloneEvent;
	}

protected:
	EventType m_EvtType;
};

enum StateFlag
{
	STOP,
	UPDATING,
	OVER,
};

/// <summary>
/// class FAbstractState
/// </summary>
class FAbstractState
{
public:
	bool m_ForceEnter;
	FAbstractState *m_ForceEnterState;
	bool m_ForceExit;
	FAbstractState *m_ForceExitState;
	FStateMachine* m_Machine;
	StateFlag m_StateFlag;
	EventType m_Type;

	CCObject* m_PlayerCar;

protected:
	list<EventType> m_HighPriority;
	list<EventType> m_LowPriority;
	FState* m_Parent;

	bool m_CanRepeat;

	FAbstractState(FState* parent)
	{
		m_Parent = parent;
		m_CanRepeat = false;

		m_ForceEnterState = NULL;
		m_ForceExitState = NULL;
		m_Machine = NULL;

		m_ForceEnter = false;
		m_ForceExit = false;

		m_StateFlag = STOP;
		m_Type = ET_None;
	}

	virtual ~FAbstractState()
	{

	}

public:

	bool CanRepeat()
	{
		return m_CanRepeat;
	}

	CCObject* PlayerCar()
	{
		return m_PlayerCar; 
	}


	// signal function call
	virtual bool Entered() = 0;
	virtual void Exited()  = 0;

	// update function
	virtual void Update(float dt) = 0;
	virtual void UpdatePhysics() = 0;

	virtual bool Reject() = 0;
};

/// <summary>
/// class FState
/// </summary>
class FState : public FAbstractState
{
public:
	/// <summary>
	/// Every child state should be either a standalone mode or parallel state. 
	/// </summary>
	enum ChildMode
	{
		/// <summary>
		/// standalone state
		/// </summary>
		ExclusiveStates,

		/// <summary>
		/// which can be execute in parallel
		/// </summary>
		ParallelStates,
	};


	FState(ChildMode childMode, FState* parent)
		: FAbstractState(parent)
	{
		childStateMode = childMode;
		m_evtClone = NULL;
	}

	virtual ~FState()
	{
		delete m_evtClone;
	}

	/// <summary>
	/// Gets or sets the child state mode.
	/// </summary>
	ChildMode ChildSateMode() {return childStateMode;}

    void CloneEvent(FEvent* evt)
	{
		delete m_evtClone;
		m_evtClone = NULL;
		m_evtClone = evt->Clone();
	}

	virtual bool Entered()
	{
		if(this->m_StateFlag == UPDATING && !m_CanRepeat)
			return false;

		m_StateFlag = UPDATING;
	    return true;
	}

	virtual void Exited()
	{
		m_StateFlag = STOP;
	}

	virtual void Update(float dt)
	{
	}

	virtual void UpdatePhysics()
	{
	}

	virtual bool Reject();

protected:

	ChildMode childStateMode; // child mode
	FEvent* m_evtClone;
};

typedef list<FState*> StateArrayList;




/// <summary>
/// GroupState, a group of states, when group state leave to another group state, all the states in 
/// current group will exit.
/// </summary>
class GroupState
{
private:
	list<FAbstractState*> m_UpdatingStates;
	list<FAbstractState*> m_EnterStates;
	FAbstractState* m_NormalState;
	CCObject* m_Car;

public:
	GroupState()
	{
		m_NormalState = NULL;
	}


	bool Init(FAbstractState* normalState)
	{
		m_Car = normalState->m_PlayerCar;
		m_NormalState = normalState;
		PushState(normalState);
		return true;
	}

	void PushState(FAbstractState* state)
	{
		if (NULL == state ||  (find(m_EnterStates.begin(), m_EnterStates.end(), state) != m_EnterStates.end()))
			return;

		m_EnterStates.push_back(state);
	}



	void Update(float dt)
	{
		list<FAbstractState*> tempList ;

		for(list<FAbstractState*>::iterator it = m_EnterStates.begin(); it != m_EnterStates.end(); it++)
		{
			FAbstractState* state = *it;
			if (!state->Reject())
			{
				tempList.push_back(state);
			}
		}


		for(list<FAbstractState*>::iterator it = m_UpdatingStates.begin(); it != m_UpdatingStates.end(); it++)
		{
			FAbstractState* state = *it;
			if (OVER == state->m_StateFlag)
			{
				state->Exited();
			}
		}

		for(list<FAbstractState*>::iterator it = tempList.begin(); it != tempList.end(); it++)
		{
			FAbstractState* state = *it;
			if (STOP == state->m_StateFlag || state->CanRepeat())
			{
				state->Entered();
			}

			if (find(m_UpdatingStates.begin(), m_UpdatingStates.end(), state) == m_UpdatingStates.end())
			{
				m_UpdatingStates.push_back(state);
			}
		}

		m_EnterStates.clear();

		for(list<FAbstractState*>::iterator it = m_UpdatingStates.begin(); it != m_UpdatingStates.end(); it++)		
		{
			FAbstractState* state = *it;
			if (UPDATING == state->m_StateFlag)
			{
				state->Update(dt);
			}
		}
	}
};


/// <summary>
/// class FStateMachine
/// </summary>
class FStateMachine
{
public:
	// event priority 
	enum EventPriority
	{
		NormalPriority, // normal  priority
		HighPriority    // high priority
	};

	// restore policy
	enum RestorePolicy
	{
		DontRestoreProperties,  // don't restore state's properties
		RestoreProperties       // restore properties
	};

public:
	FEvent* tempItem ;
	RestorePolicy m_GlobalRestorePolicy;
	bool m_Runing;

	/// <summary>
	/// Initial state.
	/// </summary>
	FAbstractState* m_Initialstate;

	/// <summary>
	/// Current state.
	/// </summary>
	FAbstractState* m_CurrentState;

	map<EventType, FState*> m_FStates;
	list<FEvent*> m_Normalevents;
	list<FEvent*> m_Highevents;

	GroupState m_GroupState;

public:
	FStateMachine()
	{
		m_GlobalRestorePolicy = DontRestoreProperties;
		m_Runing = false;
		tempItem = NULL;
		m_Initialstate = NULL;
	}

	~FStateMachine()
	{
		
	}

	RestorePolicy GlobalRestorePolicy()
	{
		return m_GlobalRestorePolicy;
	}

	void AddState(EventType type, FState* state)
	{
		m_FStates[type]= state;
		state->m_Machine = this;
	}

	void RemoveAllState(void)
	{
		for(map<EventType, FState*>::iterator iter = m_FStates.begin(); iter != m_FStates.end(); iter++)
		{
			if(iter->second != NULL)
			{
				delete iter->second;
				iter->second = NULL;
			}
		}
	}

	FState* GetState(EventType type)
	{
		FState* state = NULL;

		if (m_FStates.find(type) != m_FStates.end())
		{
			state = m_FStates[type];
		}

		return state;
	}

	bool IsRunning()
	{
		return m_Runing;
	}

	/*
	*@ prototype, PostEvent()
	*@ param, evt 
	*@ param, priority
	*@ note, post one event to change current state to the target state. it will be deleted  by the 
	*    state machine. so do not delete the event from the outside after new it.!!!!!!!
	*/
	 void  PostEvent(FEvent* evt, EventPriority priority)
	{
		switch(priority)
		{
		case NormalPriority:			
			m_Normalevents.push_back(evt);
			break;

		case HighPriority:
			m_Highevents.push_back(evt);
			break;

		default:
			m_Normalevents.push_back(evt);
			break;
		}
	}

	 void SetInitialState(FState* state)
	 {
		 m_Initialstate = state;
	 }

	 FAbstractState* InitialState()
	 {
		 return m_Initialstate;
	 }

	 bool CheckStateRuning(FState* state)
	 {
		 bool bRet = false;
		 if (state)
		 {
			 bRet = state->m_StateFlag != STOP;
		 }
		 return bRet;
	 }

	 bool CheckStateRuning(EventType type)
	 {
		 bool bRet = false;
		 FState* state = m_FStates[type];
		 if (state)
		 {
			 bRet = state->m_StateFlag != STOP;
		 }
		 return bRet;
	 }

	 void Start() 
	 {
		 if (m_Initialstate != NULL)
		 {
			 m_CurrentState = m_Initialstate;
			 m_GroupState.Init(m_CurrentState);
			 m_Runing = true;
		 }
		 else
		 {
			 CCLOG("No initial state!");
		 }
	 }

	 void Stop()
	 {
		 if (m_Initialstate != NULL)
		 {
			 m_Initialstate->Exited();
			 m_CurrentState = NULL;
			 m_Runing = false;
		 }
	 }

	 void Update(float dt)
	 {
		 if (PopOneEvent(tempItem))
		 {
			 if (m_CurrentState != NULL)
			 {
				 FState* nextState;
				 if (m_FStates.find(tempItem->GetEventType()) != m_FStates.end())
				 {
					 nextState = m_FStates[tempItem->GetEventType()];
					 nextState->CloneEvent(tempItem);
					 m_GroupState.PushState(nextState);
				 }
			 }
			 else
			 {
				 m_CurrentState = m_FStates[tempItem->GetEventType()];
				 ((FState*)m_CurrentState)->CloneEvent(tempItem);
				 m_GroupState.PushState(m_CurrentState);
			 }

			 // free the event which pushed from the external call of "PostEvent()"
			 delete tempItem;
			 tempItem = NULL;
		 }

		 m_GroupState.Update(dt);
	 }

	 void UpdatePhysics()
	 {
	 }


private:

	bool PopOneEvent(FEvent*& item)
	{
		// post high events to the current state and other active state
		if (m_Highevents.size() > 0)
		{			
			item = m_Highevents.front();
			m_Highevents.erase(m_Highevents.begin());
			return true;
		}

		if (m_Normalevents.size() > 0)
		{
			item = m_Normalevents.front();
			m_Normalevents.erase(m_Normalevents.begin());
			return true;
		}

		return false;
	}

};