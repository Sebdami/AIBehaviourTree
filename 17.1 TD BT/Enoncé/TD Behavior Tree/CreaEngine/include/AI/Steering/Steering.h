/***************************************************/
/* Nom:	Steering.h
/* Description: Steering
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __Steering_H_
#define __Steering_H_

#include "Core\Component.h"
#include "Core\Math.h"
#include "Core\GameManager.h"
#include "Graphics\Shape.h"

namespace crea
{
	// Predefinitions
	class GameManager;
	class Behaviour;

	class CREAENGINE_API PairFloatBehaviour : public pair<float, Behaviour*> 
	{
	public:
		PairFloatBehaviour(float _f, Behaviour* _p);
		~PairFloatBehaviour();
	};

	class CREAENGINE_API Steering : public Component
	{
		Vector2f steer();

	protected:
		GameManager* m_pGM;
		
		// Collider
		Collider* m_pCollider;

		// Target
		Entity* m_pTarget;
		Vector2f m_vTargetOffset;

		std::vector<PairFloatBehaviour*> m_Behaviours; // All the Behaviours and their weight associated

		Vector2f m_vSteeringDirection;

	public:
		Steering();
		virtual ~Steering();

		void setCollider(Collider* _pCollider);

		void setTarget(Entity* _pEntity) { m_pTarget = _pEntity; }
		Entity* getTarget() { return m_pTarget; }
		void setTargetOffset(Vector2f _vOffset) { m_vTargetOffset = _vOffset; }
		Vector2f getTargetOffset() { return m_vTargetOffset; }

		void addBehaviour(Behaviour* _Behaviour, float _weight);
		void removeBehaviour(Behaviour* _Behaviour);
		void clearBehaviours();

		Vector2f getSteeringDirection() { return m_vSteeringDirection; }

		string asString();

		virtual bool init();
		virtual bool update();
		virtual bool draw();
		virtual bool quit();

		virtual Component* clone() 
		{ 
			Steering* p = new Steering(*this); 
			m_Behaviours.clear();  
			return p; 
		} // CB: to do: keep Behaviours (instanciate new PairFloatBehaviour for each Behaviour)
	};

} // namespace crea

#endif
