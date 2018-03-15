/***************************************************/
/* Nom:	FSMSteeringPeon.h
/* Description: FSMSteeringPeon
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __FSMSteeringPeon_H_
#define __FSMSteeringPeon_H_

#include "AI\StateMachine.h"
#include "Scripts\CharacterController.h"
#include "Scripts\FSMSteeringPeonLive.h"

using namespace crea;

class FSMSteeringPeon : public StateMachine
{
	Entity*			m_pEntity;
	CharacterController* m_pCharacterController;
	Agent*			m_pAgent;

	bool			m_bPaused;

	int				m_iLife;
	FSMSteeringPeonLive*	m_pFSMSteeringPeonLive;

public:
	FSMSteeringPeon();
	virtual ~FSMSteeringPeon();
	
	virtual bool States(StateMachineEvent _event, Msg* _msg, int _state);

	virtual Component* clone();
};

#endif
