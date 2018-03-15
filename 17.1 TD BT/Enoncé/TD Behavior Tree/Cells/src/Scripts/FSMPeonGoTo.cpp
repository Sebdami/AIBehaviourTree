#include "stdafx.h"

#include "Scripts\FSMPeonGoTo.h"
#include "Scripts\Messages.h"
#include <string.h>


FSMPeonGoTo::FSMPeonGoTo(Vector2f _vTarget) : m_vTarget(_vTarget)
{
}

FSMPeonGoTo::~FSMPeonGoTo()
{
	for (short i = 0; i < (short)m_vPath.size(); i++)
	{
		delete m_vPath[i];
	}
	m_vPath.clear();
}

bool FSMPeonGoTo::States(StateMachineEvent _event, Msg* _msg, int _state)
{
	BeginStateMachine
		//OnMsg(MSG_Teleport)
		//SetState(STATE_CompletedPath);

		/////////////////////////////////////////////////////////////////
		State(STATE_Init)
		OnEnter
			// Get Entity
			m_pEntity = getEntity();
			// Get CharacterController
			m_pCharacterController = m_pEntity->getComponent<CharacterController>();
			// A*
			m_pMapSearch = m_pGM->getMapSearch("FSMPeonGoTo");
			// Get Agent
			m_pAgent = m_pEntity->getComponent<Agent>();
		OnUpdate
			SetState(STATE_SearchPath);
		/////////////////////////////////////////////////////////////////
		State(STATE_SearchPath)
		OnEnter
			m_pMapSearch->setStartAndGoal(m_pEntity->getPosition(), m_vTarget);
			m_pCharacterController->move(Vector2f(0.f, 0.f));

		OnUpdate
			MapSearch::SearchState s = m_pMapSearch->update();
			if (s == MapSearch::SEARCH_STATE_SUCCEEDED)
			{
				SetState(STATE_FollowPath);
			}
			else if (s == MapSearch::SEARCH_STATE_FAILED)
			{
				SetState(STATE_SearchFailed);
			}

		OnExit

		/////////////////////////////////////////////////////////////////
		State(STATE_FollowPath)
		OnEnter
			m_pMapSearch->getSolution(m_vPath);
			m_uiPathIndex = 0;
			m_pPathTarget = m_vPath[m_uiPathIndex];

		OnUpdate
			if (GoTo(*m_pPathTarget))
			{ 
				++m_uiPathIndex;
				if (m_uiPathIndex == m_vPath.size())
				{
					SetState(STATE_CompletedPath);
				}
				else
				{
					m_pPathTarget = m_vPath[m_uiPathIndex];
				}
			}

		OnExit

		/////////////////////////////////////////////////////////////////
		State(STATE_SearchFailed)
		OnEnter


		OnUpdate


		OnExit

		/////////////////////////////////////////////////////////////////
		State(STATE_CompletedPath)
		OnEnter


		OnUpdate


		OnExit
		EndStateMachine
}

bool FSMPeonGoTo::GoTo(Vector2f& _vTargetPosition)
{
	bool bArrived = false;
	Vector2f vEntityPosition = m_pEntity->getPosition();
	Vector2f vVelocity = _vTargetPosition - vEntityPosition;
	if (vVelocity.length() > 10)
	{
		m_pCharacterController->setAction(kAct_Walk);
	}
	else
	{
		bArrived = true;
		vVelocity = Vector2f(0.f, 0.0f);
	}

	// CB: adjust velocity with dexterity
	int iDexterity = m_pAgent->getDexterity() * 20; // cb: 200 pixels/second when dexterity at 10 

	vVelocity.normalize();
	m_pCharacterController->move(vVelocity * (float)iDexterity);

	return bArrived;
}