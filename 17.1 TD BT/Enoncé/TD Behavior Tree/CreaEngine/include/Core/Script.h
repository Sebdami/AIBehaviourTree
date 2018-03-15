/***************************************************/
/* Nom:	Script.h
/* Description: Script
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef _Script_H
#define _Script_H

#include "Core\Component.h"
#include "Core\PhysicsManager.h"

namespace crea
{
	class CREAENGINE_API ScriptFactory {
	public:
		virtual Script* create(string& _szName) = 0;
	};

	class CREAENGINE_API Script : public Component
	{
	public:

		Script()
		{
		}

		virtual ~Script()
		{
		}

		virtual bool init() = 0;
		virtual bool update() = 0;
		virtual bool draw() = 0;
		virtual bool quit() = 0;

		virtual void onCollisionEnter2D(Collision2D _coll) {}
		virtual void onCollisionStay2D(Collision2D _coll) {}
		virtual void onCollisionExit2D(Collision2D _coll) {}

		virtual Component* clone() = 0;
	};

} // namespace crea

#endif // _Script_H