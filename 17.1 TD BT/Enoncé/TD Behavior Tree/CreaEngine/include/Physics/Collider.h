/***************************************************/
/* Nom:	Collider.h
/* Description: Collider
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __Collider_H_
#define __Collider_H_

#include "Core\Component.h"
#include "Core\Math.h"

namespace crea
{
	enum EnumColliderType
	{
		Collider_Invalid = -1,
		Collider_Box = 0,
		Collider_Circle,
	};

	class CircleCollider;
	class BoxCollider;

	class CREAENGINE_API Collider : public Component
	{
		bool collisionBoxBox(Collider* _pCollider1, Collider* _pCollider2);
		bool collisionBoxCircle(Collider* _pCollider1, Collider* _pCollider2);
		bool collisionCircleCircle(Collider* _pCollider1, Collider* _pCollider2);

	protected:
		EnumColliderType m_eColliderType;
		bool m_bIsColliding;
		bool m_bIsTrigger;

	public:
		Collider(EnumColliderType _eColliderType = Collider_Circle);
		virtual ~Collider();

		EnumColliderType getColliderType() { return m_eColliderType; }
		bool& getIsTrigger() { return m_bIsTrigger; }

		virtual bool isColliding(Collider* _pCollider, bool _bWithTrigger = false);
		
		static Collider* loadFromFileJSON(string& _filename);
			
		virtual bool init();
		virtual bool update();
		virtual bool draw();
		virtual bool quit();

		virtual Component* clone() = 0;
	};

	class CREAENGINE_API BoxCollider : public Collider
	{
		Vector2f m_vOrigin;
		Vector2f m_vSize;
		
	public:
		BoxCollider();
		virtual ~BoxCollider();

		inline Vector2f& getOrigin() { return m_vOrigin; }
		inline Vector2f& getSize() { return m_vSize; }
		inline float getMinX() { return m_vOrigin.getX(); }
		inline float getMinY() { return m_vOrigin.getY(); }
		inline float getMaxX() { return m_vOrigin.getX() + m_vSize.getX(); }
		inline float getMaxY() { return m_vOrigin.getY() + m_vSize.getY(); }
		inline Vector2f getMin() { return Vector2f(getMinX(), getMinY()); }
		inline Vector2f getMax() { return Vector2f(getMaxX(), getMaxY()); }
	
		virtual Component* clone() { return new BoxCollider(*this); }
	};

	class CREAENGINE_API CircleCollider : public Collider
	{
		Vector2f m_vCenter;
		float m_fRadius;

	public:
		CircleCollider();
		virtual ~CircleCollider();

		inline Vector2f& getCenter() {	return m_vCenter; }
		inline Vector2f getWorldCenter() { return (m_pEntity ? m_pEntity->getPosition() + m_vCenter : m_vCenter); } // CB: for now, no rotation but to modify when Transformable with rotation
		inline float& getRadius() { return m_fRadius; }

		virtual Component* clone();
	};

} // namespace crea

#endif
