/***************************************************/
/* Nom:	FSMPeonLive.h
/* Description: FSMPeonLive
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __FSMPeonLive_H_
#define __FSMPeonLive_H_

#include "AI\StateMachine.h"
#include "Scripts\CharacterController.h"
#include "FSMPeonGoTo.h"

using namespace crea;

class FSMPeonLive : public StateMachine
{
	GameManager*	m_pGM;
	Entity*			m_pEntity;
	CharacterController* m_pCharacterController;
	Agent*			m_pAgent;

	FSMPeonGoTo*	m_pFSMPeonGoTo;

	Entity*			m_pMine;
	Entity*			m_pHQ;
	Entity*			m_pTarget;
	Vector2f		m_vTarget;

	bool			m_bPaused;
	
public:
	FSMPeonLive();
	virtual ~FSMPeonLive();

	void setEntity(Entity* _p) { m_pEntity = _p; }
	void setCharacterController(CharacterController* _p) { m_pCharacterController = _p; }

	virtual bool States(StateMachineEvent _event, Msg* _msg, int _state);

	virtual Component* clone() { return new FSMPeonLive(*this); }
};

#endif
