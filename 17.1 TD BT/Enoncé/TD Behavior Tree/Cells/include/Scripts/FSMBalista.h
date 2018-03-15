/***************************************************/
/* Nom:	FSMBalista.h
/* Description: FSMBalista
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __FSMBalista_H_
#define __FSMBalista_H_

#include "AI\StateMachine.h"
#include "Scripts\CharacterController.h"
#include "Scripts\FSMBalistaLive.h"

using namespace crea;

class FSMBalista : public StateMachine
{
	Entity*			m_pEntity;
	CharacterController* m_pCharacterController;
	Agent*			m_pAgent;

	bool			m_bPaused;

	int				m_iLife;
	FSMBalistaLive*	m_pFSMBalistaLive;

public:
	FSMBalista();
	virtual ~FSMBalista();
	
	virtual bool States(StateMachineEvent _event, Msg* _msg, int _state);

	virtual Component* clone() { return new FSMBalista(*this); }
};

#endif
