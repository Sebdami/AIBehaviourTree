/***************************************************/
/* Nom:	UserController.h
/* Description: UserController
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __UserController_H_
#define __UserController_H_

#include "Core\Script.h"
#include "Core\Math.h"
using namespace crea;

class CharacterController;

class UserController : public Script
{
	GameManager* m_pGM;
	CharacterController* m_pCharacterController;

	Vector2f m_vDirection;

public:
	UserController();
	virtual ~UserController();

	inline void setCharacterController(CharacterController* _pCharacterController);

	virtual bool init();
	virtual bool update();
	virtual bool draw();
	virtual bool quit();

	virtual void onCollisionEnter2D(Collision2D _coll) { return; }

	virtual Component* clone() { return new UserController(*this); }
};


#endif
