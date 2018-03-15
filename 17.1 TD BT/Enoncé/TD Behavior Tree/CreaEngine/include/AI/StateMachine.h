/***************************************************/
/* Nom:	StateMachine.h
/* Description: StateMachine
/* Auteur: BRUNEAU Colin
/***************************************************/
// Portions Copyright (C) Steve Rabin, 2001
#ifndef __STATEMCH_H__
#define __STATEMCH_H__

#include <assert.h>
#include <deque>
#include "AI.h"
#include "Msg.h"
#include "Core\EntityManager.h"
#include "Core\DebugLog.h"
#include "Core\TimeManager.h"
#include "Core\Script.h"

#define MAX_STATES_IN_STACK 20

namespace crea
{

//Scene Machine Language Macros (put these keywords in the file USERTYPE.DAT in the same directory as MSDEV.EXE)
#define BeginStateMachine		if( _state < 0 ) { char statename[64] = "STATE_Global"; if(0) {
#define EndStateMachine			return( true ); } } else { assert( 0 && "Invalid Scene" ); return( false ); } return( false );
#define State(a)				return( true ); } } else if( a == _state ) { char statename[64] = #a; if(0) { 
#define OnMsg(a)				return( true ); } else if( EVENT_Message == _event && _msg && a == _msg->GetMsgName() ) { g_debuglog->LogStateMachineEvent( getEntity()->GetID(), _msg, statename, #a, true );
#define OnOtherMsg()			return( true ); } else {
#define OnEvent(a)				return( true ); } else if( a == _event ) { g_debuglog->LogStateMachineEvent( getEntity()->GetID(), _msg, statename, #a, true );
#define OnUpdate				OnEvent( EVENT_Update )
#define OnEnter					OnEvent( EVENT_Enter )
#define OnExit					OnEvent( EVENT_Exit )



	enum StateMachineEvent {
		EVENT_INVALID,
		EVENT_Update,
		EVENT_Message,
		EVENT_Enter,
		EVENT_Exit
	};

	typedef enum {
		NO_MSG_SCOPING,
		SCOPE_TO_THIS_STATE
	} MSG_Scope;


	class CREAENGINE_API StateMachine : public Script
	{
	public:

		StateMachine();
		virtual ~StateMachine();

		void Initialize(Entity* _pOwner);
		void Update(void);
		void SetState(unsigned int _state);
		int SetStateInHistory();

		void SendMsg(int name, objectID receiver, void* data = NULL);
		void SendDelayedMsg(float delay, int name, objectID receiver, void* data = NULL);
		void SendDelayedMsgToMe(float delay, int name, MSG_Scope scope = NO_MSG_SCOPING);

		int GetState(void) { return(m_currentState); }
		double GetTimeInState(void) { return(g_time->getGameTime().asSeconds() - m_timeOnEnter); }

		void SetCCReceiver(objectID id) { m_ccMessagesToAgent = id; }
		void ClearCCReceiver(void) { m_ccMessagesToAgent = 0; }
		objectID GetCCReceiver(void) { return(m_ccMessagesToAgent); }

		void Process(StateMachineEvent event, Msg * msg);

		virtual bool init() { Initialize(getEntity()); return true; }
		virtual bool update() { Update(); return true; }
		virtual bool draw() { return true; }
		virtual bool quit() { return true; }


	private:

		unsigned int m_currentState;
		unsigned int m_nextState;
		bool m_stateChange;
		double m_timeOnEnter;
		objectID m_ccMessagesToAgent;
		std::deque<unsigned int> m_stack;

		virtual bool States(StateMachineEvent _event, Msg* _msg, int _state) = 0;

		virtual Component* clone() = 0;
	};

}

#endif	// __STATEMCH_H__