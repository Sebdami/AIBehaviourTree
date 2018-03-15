#include "stdafx.h"

#include "Scripts\FSMBalistaLive.h"
#include "Scripts\Messages.h"
#include <string.h>


//Add new states here
enum States {
	STATE_Init,
	STATE_Idle,
	STATE_GoTo,
	STATE_GetResource,
	STATE_DropResource,
	STATE_Hit,
	STATE_Build,
	STATE_Mine,
	STATE_Harvest,
	STATE_Kill
};

FSMBalistaLive::FSMBalistaLive()
{
}

FSMBalistaLive::~FSMBalistaLive()
{

}

bool FSMBalistaLive::States(StateMachineEvent _event, Msg* _msg, int _state)
{
	BeginStateMachine

		OnMsg(MSG_Stop)
			m_pCharacterController->setAction(kAct_Default);
			m_bPaused = true;

		OnMsg(MSG_GoTo)
			m_pCharacterController->setCondition(kACond_Default);
			SetState(STATE_GoTo);
			m_bPaused = false;

		OnMsg(MSG_Kill)
			SetState(STATE_Kill);

		OnMsg(MSG_Hit)
			SetState(STATE_Hit);

		OnMsg(MSG_HitStop)
			SetStateInHistory();

		///////////////////////////////////////////////////////////////
		State(STATE_Init)
		OnEnter
			// Get Entity
			m_pEntity = getEntity();
			// Get CharacterController
			m_pCharacterController = m_pEntity->getComponent<CharacterController>();
			// Get Agent
			m_pAgent = m_pEntity->getComponent<Agent>();

		OnUpdate
			SetState(STATE_Idle);
				
		///////////////////////////////////////////////////////////////
		State(STATE_Idle)
		OnEnter
			m_bPaused = false;

		OnUpdate
			// Mouse control
			if (m_pGM->isMouseButtonPressed(Button::MouseRight))
			{
				m_vTarget = m_pGM->getMousePosition();
				m_pCharacterController->setCondition(kACond_Default);
				// CB: for now do nothing...
				//SetState(STATE_GoTo);
			}

		///////////////////////////////////////////////////////////////
		State(STATE_GoTo)
		OnEnter
			m_pFSMBalistaGoTo = new FSMBalistaGoTo(m_vTarget);
			m_pFSMBalistaGoTo->Initialize(getEntity());

		OnUpdate
			if (!m_bPaused)
			{
				m_pFSMBalistaGoTo->Update();
				if (m_pFSMBalistaGoTo->GetState() == FSMBalistaGoTo::STATE_CompletedPath)
				{
					SetState(STATE_Idle);
				}
				else if (m_pFSMBalistaGoTo->GetState() == FSMBalistaGoTo::STATE_SearchFailed)
				{
					SetState(STATE_Idle);
				}
			}
			else
			{
				m_pCharacterController->move(Vector2f(0.f, 0.f));
			}
		OnExit
			delete m_pFSMBalistaGoTo;
			m_pFSMBalistaGoTo = nullptr;

		///////////////////////////////////////////////////////////////
		State(STATE_Hit)
		OnEnter
			// CB: a hit is a temporary death...
			SendDelayedMsgToMe(0.5f, MSG_HitStop);

		OnUpdate
			m_pCharacterController->setCondition(kACond_Default);
			m_pCharacterController->setAction(kAct_Die);

		OnExit
			m_pCharacterController->setCondition(kACond_Default);
			m_pCharacterController->setAction(kAct_Default);

		///////////////////////////////////////////////////////////////
		State(STATE_Kill)
		OnEnter
			m_pCharacterController->setCondition(kACond_Default);
			m_pCharacterController->setAction(kAct_Chop);

		///////////////////////////////////////////////////////////////
		State(STATE_Hit)
		OnEnter
			// CB: a hit is a temporary death...
			SendDelayedMsgToMe(0.5f, MSG_HitStop);

		OnUpdate
			m_pCharacterController->setCondition(kACond_Default);
			m_pCharacterController->setAction(kAct_Die);

		OnExit
			m_pCharacterController->setCondition(kACond_Default);
			m_pCharacterController->setAction(kAct_Default);

	EndStateMachine
}


