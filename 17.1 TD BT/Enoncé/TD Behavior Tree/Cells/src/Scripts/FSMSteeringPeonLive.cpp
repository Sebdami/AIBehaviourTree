#include "stdafx.h"

#include "Scripts\FSMSteeringPeonLive.h"
#include "Scripts\Messages.h"
#include <string.h>
#include "AI\Steering\Behaviour.h"


//Add new states here
enum States {
	STATE_Init,
	STATE_Idle,
	STATE_Seek,
	STATE_Flee,
	STATE_Pursuit,
	STATE_Evasion,
	STATE_Arrival,
	STATE_Wander,
	STATE_PathFollowingIdle,
	STATE_PathFollowing,
	STATE_UCA
};

FSMSteeringPeonLive::FSMSteeringPeonLive()
{
}

FSMSteeringPeonLive::~FSMSteeringPeonLive()
{

}

bool FSMSteeringPeonLive::States(StateMachineEvent _event, Msg* _msg, int _state)
{
	BeginStateMachine

		OnMsg(MSG_Stop)
			m_pCharacterController->setAction(kAct_Default);
			m_bPaused = true;

		OnMsg(MSG_GoTo)
			m_pCharacterController->setCondition(kACond_Default);
			SetState(STATE_Seek);
			m_bPaused = false;

		OnMsg(MSG_Seek)
			m_pCharacterController->setCondition(kACond_Default);
			SetState(STATE_Seek);
			m_bPaused = false;

		OnMsg(MSG_Flee)
			m_pCharacterController->setCondition(kACond_Default);
			SetState(STATE_Flee);
			m_bPaused = false;

		OnMsg(MSG_Pursuit)
			m_pCharacterController->setCondition(kACond_Default);
			SetState(STATE_Pursuit);
			m_bPaused = false;

		OnMsg(MSG_Evasion)
			m_pCharacterController->setCondition(kACond_Default);
			SetState(STATE_Evasion);
			m_bPaused = false;

		OnMsg(MSG_Arrival)
			m_pCharacterController->setCondition(kACond_Default);
			SetState(STATE_Arrival);
			m_bPaused = false;

		OnMsg(MSG_Wander)
			m_pCharacterController->setCondition(kACond_Default);
			SetState(STATE_Wander);
			m_bPaused = false;

		OnMsg(MSG_PathFollowing)
			m_pCharacterController->setCondition(kACond_Default);
			SetState(STATE_PathFollowingIdle);
			m_bPaused = false;

		OnMsg(MSG_UCA)
			m_pCharacterController->setCondition(kACond_Default);
			SetState(STATE_UCA);
			m_bPaused = false;

		///////////////////////////////////////////////////////////////
		State(STATE_Init)
		OnEnter
			// Get Game Manager
			m_pGM = GameManager::getSingleton();
			// Get Entity
			m_pEntity = getEntity();
			// Get CharacterController
			m_pCharacterController = m_pEntity->getComponent<CharacterController>();
			// Get Agent
			m_pAgent = m_pEntity->getComponent<Agent>();
			// Get Steering
			m_pSteering = m_pEntity->getComponent<Steering>();
			m_pSteering->init();
			// Get Vehicle
			m_pVehicle = m_pEntity->getComponent<Vehicle>();
			m_pVehicle->setUpdatePosition(false); //The characterController will update the position

		OnUpdate
			SetState(STATE_Idle);
				
		///////////////////////////////////////////////////////////////
		State(STATE_Idle)
		OnEnter
			m_bPaused = false;
			m_pCharacterController->setCondition(kACond_Default);
		OnUpdate
			Move();

		///////////////////////////////////////////////////////////////
		State(STATE_Seek)
		OnEnter
			m_pSteering->clearBehaviours();
			m_pSteering->addBehaviour(new Seek(getEntity(), m_pGM->getEntity("mouse")), 1.0f);

		OnUpdate
			Move();

		OnExit
			m_pSteering->clearBehaviours();

		///////////////////////////////////////////////////////////////
		State(STATE_Flee)
			OnEnter
			m_pSteering->clearBehaviours();
			m_pSteering->addBehaviour(new Flee(getEntity(), m_pGM->getEntity("mouse")), 1.0f);

		OnUpdate
			Move();

		OnExit
			m_pSteering->clearBehaviours();

		///////////////////////////////////////////////////////////////
		State(STATE_Pursuit)
			OnEnter
			m_pSteering->clearBehaviours();
			m_pSteering->addBehaviour(new Pursuit(getEntity(), m_pGM->getEntity("mouse"), 0.5f), 1.0f);

		OnUpdate
			Move();

		OnExit
			m_pSteering->clearBehaviours();

		///////////////////////////////////////////////////////////////
		State(STATE_Evasion)
			OnEnter
			m_pSteering->clearBehaviours();
			m_pSteering->addBehaviour(new Evasion(getEntity(), m_pGM->getEntity("mouse"), 0.5f), 1.0f);

		OnUpdate
			Move();

		OnExit
			m_pSteering->clearBehaviours();

		///////////////////////////////////////////////////////////////
		State(STATE_Arrival)
			OnEnter
			m_pSteering->clearBehaviours();
			m_pSteering->addBehaviour(new Arrival(getEntity(), m_pGM->getEntity("mouse"), 100.0f), 1.0f);

		OnUpdate
			Move();

		OnExit
			m_pSteering->clearBehaviours();

		///////////////////////////////////////////////////////////////
		State(STATE_Wander)
		OnEnter
			m_pSteering->clearBehaviours();
			m_pSteering->addBehaviour(new Wander(getEntity(), 200.0f, 100.0f, 10.0f), 1.0f);

		OnUpdate
			Move();

		OnExit
			m_pSteering->clearBehaviours();

		///////////////////////////////////////////////////////////////
		State(STATE_PathFollowingIdle)
		OnEnter
			m_bPaused = false;
			m_pCharacterController->setCondition(kACond_Default);
		OnUpdate
			m_pCharacterController->move(Vector2f(0.f, 0.0f));
			m_pVehicle->setVelocity(Vector2f(0.f, 0.0f));

			if (InputManager::getSingleton()->isMouseButtonPressed(MouseRight))
			{
				m_pTarget = m_pGM->getEntity("mouse");
				m_vTarget = m_pTarget->getPosition();
				SetState(STATE_PathFollowing);
			}

		///////////////////////////////////////////////////////////////
		State(STATE_PathFollowing)
		OnEnter
			m_pFSMSteeringPeonGoTo = new FSMSteeringPeonGoTo(m_vTarget);
			m_pFSMSteeringPeonGoTo->Initialize(getEntity());

		OnUpdate
			m_pFSMSteeringPeonGoTo->Update();
			if (m_pFSMSteeringPeonGoTo->GetState() == FSMSteeringPeonGoTo::STATE_CompletedPath)
			{
				SetState(STATE_PathFollowingIdle);
			}

		OnExit
			delete m_pFSMSteeringPeonGoTo;
			m_pFSMSteeringPeonGoTo = nullptr;


		///////////////////////////////////////////////////////////////
		State(STATE_UCA)
			OnEnter
			m_pOthers.clear();
			Entity* pOther = m_pGM->getEntity("balista1");
			m_pOthers.push_back(pOther);
			m_pSteering->clearBehaviours();
			m_pSteering->addBehaviour(new Seek(getEntity(), m_pGM->getEntity("mouse")), 1.0f);
			m_pSteering->addBehaviour(new UnalignedCollisionAvoidance(getEntity(), 32.0f, 1.0f, &m_pOthers), 1.0f);

		OnUpdate
			
			Move();

		OnExit
			m_pSteering->clearBehaviours();

	EndStateMachine
}

bool FSMSteeringPeonLive::Move()
{
	bool bMoving = true;
	Vector2f vVelocity = m_pVehicle->getVelocity();
	if (vVelocity.length() > 1)
	{
		m_pCharacterController->setAction(kAct_Walk);
	}
	else
	{
		bMoving = false;
		vVelocity = Vector2f(0.f, 0.0f);
	}

	// CB: adjust velocity with dexterity
	int iDexterity = m_pAgent->getDexterity() / 5;
	m_pCharacterController->move(vVelocity * (float)iDexterity);

	return bMoving;
}

Component* FSMSteeringPeonLive::clone()
{ 
	FSMSteeringPeonLive* p = new FSMSteeringPeonLive(*this); 
	return p; 
}

