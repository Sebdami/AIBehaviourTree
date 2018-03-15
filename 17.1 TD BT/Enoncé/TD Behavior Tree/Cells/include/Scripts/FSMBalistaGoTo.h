/***************************************************/
/* Nom:	FSMBalistaGoTo.h
/* Description: FSMBalistaGoTo
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __FSMBalistaGoTo_H_
#define __FSMBalistaGoTo_H_

#include "AI\StateMachine.h"
#include "AI\Pathfinding\MapSearchManager.h"
#include "Scripts\CharacterController.h"

using namespace crea;

class FSMBalistaGoTo : public StateMachine
{
	GameManager*	m_pGM;
	Entity*			m_pEntity;
	CharacterController* m_pCharacterController;
	Agent*			m_pAgent;
	MapSearch*		m_pMapSearch;
	Vector2f		m_vTarget;

	VectorVector2f	m_vPath;
	unsigned int	m_uiPathIndex;
	Vector2f*		m_pPathTarget;
	Vector2f		m_TargetWithOffset;
	Vector2f		m_Offset;

	bool GoTo(Vector2f& _vTargetPosition);

public:
	FSMBalistaGoTo(Vector2f _vTarget);
	virtual ~FSMBalistaGoTo();

	void setEntity(Entity* _p) { m_pEntity = _p; }
	void setCharacterController(CharacterController* _p) { m_pCharacterController = _p; }

	inline VectorVector2f* getPath() { return &m_vPath; }

	virtual bool States(StateMachineEvent _event, Msg* _msg, int _state);

	virtual Component* clone() { return new FSMBalistaGoTo(*this); }

	enum States {
		STATE_Init,
		STATE_SearchPath,
		STATE_FollowPath,
		STATE_SearchFailed,
		STATE_CompletedPath
	};
};

#endif
