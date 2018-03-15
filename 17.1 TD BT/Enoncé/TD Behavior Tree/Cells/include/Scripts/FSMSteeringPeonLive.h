/***************************************************/
/* Nom:	FSMSteeringPeonLive.h
/* Description: FSMSteeringPeonLive
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __FSMSteeringPeonLive_H_
#define __FSMSteeringPeonLive_H_

#include "AI\StateMachine.h"
#include "Scripts\CharacterController.h"
#include "FSMSteeringPeonGoTo.h"
#include "AI\Steering\Steering.h"
#include "AI\Steering\Vehicle.h"

using namespace crea;

class FSMSteeringPeonLive : public StateMachine
{
	GameManager*	m_pGM;
	Entity*			m_pEntity;
	CharacterController* m_pCharacterController;
	Agent*			m_pAgent;
	Steering*		m_pSteering;
	Vehicle*		m_pVehicle;

	FSMSteeringPeonGoTo*	m_pFSMSteeringPeonGoTo;

	vector<Entity*> m_pOthers;

	Entity*			m_pTarget;
	Vector2f		m_vTarget;

	bool			m_bPaused;
	
public:
	FSMSteeringPeonLive();
	virtual ~FSMSteeringPeonLive();

	void setEntity(Entity* _p) { m_pEntity = _p; }
	void setCharacterController(CharacterController* _p) { m_pCharacterController = _p; }

	virtual bool States(StateMachineEvent _event, Msg* _msg, int _state);

	bool Move();

	virtual Component* clone();
};

#endif
