/***************************************************/
/* Nom:	FSMPeon.h
/* Description: FSMPeon
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __FSMPeon_H_
#define __FSMPeon_H_

#include "AI\StateMachine.h"
#include "Scripts\CharacterController.h"
#include "Scripts\FSMPeonLive.h"

using namespace crea;

class FSMPeon : public StateMachine
{
	Entity*			m_pEntity;
	CharacterController* m_pCharacterController;
	Agent*			m_pAgent;

	bool			m_bPaused;

	int				m_iLife;
	FSMPeonLive*	m_pFSMPeonLive;

public:
	FSMPeon();
	virtual ~FSMPeon();
	
	virtual bool States(StateMachineEvent _event, Msg* _msg, int _state);

	virtual Component* clone() { return new FSMPeon(*this); }
};

#endif
