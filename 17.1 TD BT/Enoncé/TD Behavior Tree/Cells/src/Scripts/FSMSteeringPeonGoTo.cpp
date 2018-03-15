#include "stdafx.h"

#include "Scripts\FSMSteeringPeonGoTo.h"
#include "Scripts\Messages.h"
#include <string.h>
#include "AI\Steering\Behaviour.h"


FSMSteeringPeonGoTo::FSMSteeringPeonGoTo(Vector2f _vTarget) : m_vTarget(_vTarget)
{
}

FSMSteeringPeonGoTo::~FSMSteeringPeonGoTo()
{
	for (short i = 0; i < (short)m_vPath.size(); i++)
	{
		delete m_vPath[i];
	}
	m_vPath.clear();
}

bool FSMSteeringPeonGoTo::States(StateMachineEvent _event, Msg* _msg, int _state)
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
			m_pMapSearch = m_pGM->getMapSearch("FSMSteeringPeonGoTo");
			// Get Agent
			m_pAgent = m_pEntity->getComponent<Agent>();
			// Get Steering
			m_pSteering = m_pEntity->getComponent<Steering>();
			// Get Vehicle
			m_pVehicle = m_pEntity->getComponent<Vehicle>();

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
			m_pPathTarget = m_vPath[m_vPath.size()-1];

			m_pTarget = m_pGM->getEntity("target");
			m_pTarget->setPosition(*m_vPath[0]);
			m_pSteering->setTarget(m_pTarget);

			m_pSteering->clearBehaviours();
			m_pSteering->addBehaviour(new PathFollowing(getEntity(), m_pTarget, 50.f, &m_vPath), 1.0f);

		OnUpdate
			
			if (GoTo(*m_pPathTarget))
			{ 
				SetState(STATE_CompletedPath);
			}

		OnExit
			m_pSteering->clearBehaviours();

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

bool FSMSteeringPeonGoTo::GoTo(Vector2f& _vTargetPosition)
{
	bool bArrived = false;
	Vector2f vVelocity = m_pVehicle->getVelocity();
	Vector2f vDistanceLeft = _vTargetPosition - m_pEntity->getPosition();
	if (vDistanceLeft.length() > 10)
	{
		m_pCharacterController->setAction(kAct_Walk);
	}
	else
	{
		bArrived = true;
		vVelocity = Vector2f(0.f, 0.0f);
	}
	
	// CB: adjust velocity with dexterity
	int iDexterity = m_pAgent->getDexterity() / 10;
	m_pCharacterController->move(vVelocity * (float)iDexterity);

	return bArrived;
}