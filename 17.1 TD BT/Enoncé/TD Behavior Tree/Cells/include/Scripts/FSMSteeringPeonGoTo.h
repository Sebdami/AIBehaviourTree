/***************************************************/
/* Nom:	FSMSteeringPeonGoTo.h
/* Description: FSMSteeringPeonGoTo
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __FSMSteeringPeonGoTo_H_
#define __FSMSteeringPeonGoTo_H_

#include "AI\StateMachine.h"
#include "AI\Pathfinding\MapSearchManager.h"
#include "Scripts\CharacterController.h"

using namespace crea;

class FSMSteeringPeonGoTo : public StateMachine
{
	GameManager*	m_pGM;
	Entity*			m_pEntity;
	CharacterController* m_pCharacterController;
	Agent*			m_pAgent;
	MapSearch*		m_pMapSearch;
	Vector2f		m_vTarget;

	VectorVector2f	m_vPath;
	Vector2f*		m_pPathTarget;

	Steering*		m_pSteering;
	Vehicle*		m_pVehicle;
	Entity*			m_pTarget;

	bool GoTo(Vector2f& _vTargetPosition);

public:
	FSMSteeringPeonGoTo(Vector2f _vTarget);
	virtual ~FSMSteeringPeonGoTo();

	void setEntity(Entity* _p) { m_pEntity = _p; }
	void setCharacterController(CharacterController* _p) { m_pCharacterController = _p; }

	inline VectorVector2f* getPath() { return &m_vPath; }

	virtual bool States(StateMachineEvent _event, Msg* _msg, int _state);

	virtual Component* clone() { return new FSMSteeringPeonGoTo(*this); }

	enum States {
		STATE_Init,
		STATE_SearchPath,
		STATE_FollowPath,
		STATE_SearchFailed,
		STATE_CompletedPath
	};
};

#endif
