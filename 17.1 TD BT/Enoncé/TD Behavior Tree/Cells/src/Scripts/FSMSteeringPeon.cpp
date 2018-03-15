#include "stdafx.h"

#include "Scripts\FSMSteeringPeon.h"
#include "Scripts\Messages.h"
#include <string.h>


//Add new states here
enum States {
	STATE_Spawn,
	STATE_Live,
	STATE_Die,
};

FSMSteeringPeon::FSMSteeringPeon()
{

}

FSMSteeringPeon::~FSMSteeringPeon()
{

}

bool FSMSteeringPeon::States(StateMachineEvent _event, Msg* _msg, int _state)
{
	BeginStateMachine

		OnMsg(MSG_Reset)
			SetState(STATE_Spawn);
	
		OnMsg(MSG_Die)
			SetState(STATE_Die);		
		
		OnMsg(MSG_Hit)
			m_iLife -= 20;
			if (m_iLife <= 0)
			{
				SetState(STATE_Die);
			}
			if (m_pFSMSteeringPeonLive)
			{
				m_pFSMSteeringPeonLive->States(_event, _msg, _state); // CB: propagate msg to sub-state
			}

		OnMsg(MSG_Boost)
			m_pAgent->setDexterity(m_pAgent->getDexterity()+1);

		OnOtherMsg()
			if (m_pFSMSteeringPeonLive)
			{
				m_pFSMSteeringPeonLive->States(_event, _msg, _state); // CB: propagate msg to sub-state
			}

		///////////////////////////////////////////////////////////////
		State(STATE_Spawn)
		OnEnter
			// Get Entity
			m_pEntity = getEntity();
			// Get CharacterController
			m_pCharacterController = m_pEntity->getComponent<CharacterController>();
			// Get Agent
			m_pAgent = m_pEntity->getComponent<Agent>();

			m_iLife = 100;

		OnUpdate
			SetState(STATE_Live);

		///////////////////////////////////////////////////////////////
		State(STATE_Live)
		OnEnter
			m_pFSMSteeringPeonLive = new FSMSteeringPeonLive();
			m_pFSMSteeringPeonLive->Initialize(getEntity());

		OnUpdate
			m_pFSMSteeringPeonLive->Update();
			if (m_iLife <= 0)
			{
				SetState(STATE_Die);
			}

		OnExit
			delete m_pFSMSteeringPeonLive;
			m_pFSMSteeringPeonLive = nullptr;
			
		///////////////////////////////////////////////////////////////
		State(STATE_Die)
		OnUpdate
			m_pCharacterController->setCondition(kACond_Default);
			m_pCharacterController->setAction(kAct_Die);

EndStateMachine
}

Component* FSMSteeringPeon::clone()
{
	FSMSteeringPeon* p = new FSMSteeringPeon(*this);
	return p;
}