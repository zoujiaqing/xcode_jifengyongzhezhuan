#pragma once

#ifndef PANDAXCL_SIMPLE_STATE_MACHINE_LOG

#define BeginStateMachine	if( state < 0 ) { if(0) {
#define EndStateMachine		return( true ); } } else { assert(0&&"state machine has no state"); return( false ); }  return( false );
#define State(a)			return( true ); } } else if( a == state ) { if(0) {
#define OnEvent(a)			return( true ); } else if( a == event ) {
#define OnEnter				OnEvent( EVENT_Enter )
#define OnUpdate			OnEvent( EVENT_Update )
#define OnExit				OnEvent( EVENT_Exit )

#else

#define BeginStateMachine	if( state < 0 ) { char statename[]="STATE_Global"; if(0) {
#define EndStateMachine		return( true ); } } else { assert(0&&"state machine has no state"); return( false ); }  return( false );
#define State(a)			return( true ); } } else if( a == state ) { char statename[]=#a; if(0) {
#define OnEvent(a)			return( true ); } else if( a == event ) { char eventname[]=#a; PANDAXCL_SIMPLE_STATE_MACHINE_LOG(statename, eventname);
#define OnEnter				OnEvent( EVENT_Enter )
#define OnUpdate			OnEvent( EVENT_Update )
#define OnExit				OnEvent( EVENT_Exit )

#endif