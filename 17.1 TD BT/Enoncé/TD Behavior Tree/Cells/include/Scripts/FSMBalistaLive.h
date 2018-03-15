/***************************************************/
/* Nom:	FSMBalistaLive.h
/* Description: FSMBalistaLive
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __FSMBalistaLive_H_
#define __FSMBalistaLive_H_

#include "AI\StateMachine.h"
#include "Scripts\CharacterController.h"
#include "FSMBalistaGoTo.h"

using namespace crea;

class FSMBalistaLive : public StateMachine
{
	GameManager*	m_pGM;
	Entity*			m_pEntity;
	CharacterController* m_pCharacterController;
	Agent*			m_pAgent;

	FSMBalistaGoTo*	m_pFSMBalistaGoTo;

	Vector2f		m_vTarget;

	bool			m_bPaused;
	
public:
	FSMBalistaLive();
	virtual ~FSMBalistaLive();

	void setEntity(Entity* _p) { m_pEntity = _p; }
	void setCharacterController(CharacterController* _p) { m_pCharacterController = _p; }

	virtual bool States(StateMachineEvent _event, Msg* _msg, int _state);

	virtual Component* clone() { return new FSMBalistaLive(*this); }
};

#endif
