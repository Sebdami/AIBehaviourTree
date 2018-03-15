#include "stdafx.h"

#include "Scripts\FSMBalistaGoTo.h"
#include "Scripts\Messages.h"
#include <string.h>


FSMBalistaGoTo::FSMBalistaGoTo(Vector2f _vTarget) : m_vTarget(_vTarget)
{
}

FSMBalistaGoTo::~FSMBalistaGoTo()
{
	for (short i = 0; i < (short)m_vPath.size(); i++)
	{
		delete m_vPath[i];
	}
	m_vPath.clear();
}

bool FSMBalistaGoTo::States(StateMachineEvent _event, Msg* _msg, int _state)
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
			m_pMapSearch = m_pGM->getMapSearch("FSMBalistaGoTo");
			// Get Agent
			m_pAgent = m_pEntity->getComponent<Agent>();

			// Set current agent for AA*
			MapSearchManager::getSingleton()->setCurrentAgent(m_pAgent);
			// Offset graphique
			m_Offset = Vector2f(16.0f, 16.0f);

		OnUpdate
			SetState(STATE_SearchPath);
		/////////////////////////////////////////////////////////////////
		State(STATE_SearchPath)
		OnEnter
			m_pMapSearch->setStartAndGoal(m_pEntity->getPosition()- m_Offset, m_vTarget);
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
			m_TargetWithOffset = *m_pPathTarget + m_Offset;

		OnUpdate
			if (GoTo(m_TargetWithOffset))
			{ 
				++m_uiPathIndex;
				if (m_uiPathIndex == m_vPath.size())
				{
					SetState(STATE_CompletedPath);
				}
				else
				{
					m_pPathTarget = m_vPath[m_uiPathIndex];
					m_TargetWithOffset = *m_pPathTarget + m_Offset;
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

bool FSMBalistaGoTo::GoTo(Vector2f& _vTargetPosition)
{
	bool bArrived = false;
	Vector2f vEntityPosition = m_pEntity->getPosition();
	Vector2f vVelocity = _vTargetPosition - vEntityPosition;
	if (vVelocity.length() > 1)
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