#include "stdafx.h"

#include "Scripts\FSMPeonLive.h"
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

FSMPeonLive::FSMPeonLive()
{
}

FSMPeonLive::~FSMPeonLive()
{

}

bool FSMPeonLive::States(StateMachineEvent _event, Msg* _msg, int _state)
{
	BeginStateMachine

		OnMsg(MSG_Stop)
			m_pCharacterController->setAction(kAct_Default);
			m_bPaused = true;

		OnMsg(MSG_GoToWithGold)
			m_pCharacterController->setCondition(kACond_Gold);
			SetState(STATE_GoTo);
			m_bPaused = false;

		OnMsg(MSG_GoToWithLumber)
			m_pCharacterController->setCondition(kACond_Lumber);
			SetState(STATE_GoTo);
			m_bPaused = false;

		OnMsg(MSG_GoTo)
			m_pCharacterController->setCondition(kACond_Default);
			SetState(STATE_GoTo);
			m_bPaused = false;

		OnMsg(MSG_Build)
			SetState(STATE_Build);

		OnMsg(MSG_Mine)
			SetState(STATE_Mine);

		OnMsg(MSG_Harvest)
			SetState(STATE_Harvest);

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
			// Get HQ
			m_pHQ = m_pGM->getEntity("hq1");
			// Get mine
			m_pMine = m_pGM->getEntity("mine1");

		OnUpdate
			SetState(STATE_Idle);
				
		///////////////////////////////////////////////////////////////
		State(STATE_Idle)
		OnEnter
			m_bPaused = false;

		OnUpdate
			if (m_pMine)
			{
				m_pTarget = m_pMine;
				m_vTarget = m_pTarget->getPosition() + Vector2f(-50.f, 40.f);
				m_pCharacterController->setCondition(kACond_Default);
				SetState(STATE_GoTo);
			}

		///////////////////////////////////////////////////////////////
		State(STATE_GoTo)
		OnEnter
			m_pFSMPeonGoTo = new FSMPeonGoTo(m_vTarget);
			m_pFSMPeonGoTo->Initialize(getEntity());

		OnUpdate
			if (!m_bPaused)
			{
				m_pFSMPeonGoTo->Update();
				if (m_pFSMPeonGoTo->GetState() == FSMPeonGoTo::STATE_CompletedPath)
				{
					if (m_pTarget == m_pMine)
						SetState(STATE_GetResource);
					else
						SetState(STATE_DropResource);
				}
			}
			else
			{
				m_pCharacterController->move(Vector2f(0.f, 0.f));
			}
		OnExit
			delete m_pFSMPeonGoTo;
			m_pFSMPeonGoTo = nullptr;

		///////////////////////////////////////////////////////////////
		State(STATE_GetResource)
		OnEnter
			m_pTarget = m_pHQ;
			m_vTarget = m_pTarget->getPosition() + Vector2f(40.f, 60.f);
			m_pCharacterController->setCondition(kACond_Gold);

		OnUpdate
			SetState(STATE_GoTo);

		///////////////////////////////////////////////////////////////
		State(STATE_DropResource)
		OnUpdate
			SetState(STATE_Idle);

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
		State(STATE_Build)
		OnEnter
			m_pCharacterController->setCondition(kACond_Default);
			m_pCharacterController->setAction(kAct_Chop);

		///////////////////////////////////////////////////////////////
		State(STATE_Mine)
		OnUpdate
			m_pCharacterController->setCondition(kACond_Default);
		m_pCharacterController->setAction(kAct_Chop);

		///////////////////////////////////////////////////////////////
		State(STATE_Harvest)
		OnUpdate
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


