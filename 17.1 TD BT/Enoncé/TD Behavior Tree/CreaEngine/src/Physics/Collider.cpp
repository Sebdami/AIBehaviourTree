#include "stdafx.h"

#include "Physics\Collider.h"
#include "Graphics\Sprite.h"
#include "json/json.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace crea
{
	Collider::Collider(EnumColliderType _eColliderType)
	{
		m_bIsColliding = false;
		m_bIsTrigger = false;
		m_eColliderType = _eColliderType;
	}

	Collider::~Collider()
	{
	}

	// Is the collider colliding with the given one
	bool Collider::isColliding(Collider* _pCollider, bool _bWithTrigger)
	{
		if (m_bIsTrigger && !_bWithTrigger)
		{
			return false;
		}

		if (m_eColliderType == Collider_Box)
		{
			if (_pCollider->m_eColliderType == Collider_Box)
			{
				m_bIsColliding = collisionBoxBox(this, _pCollider);
			}
			else if (_pCollider->m_eColliderType == Collider_Circle)
			{
				m_bIsColliding = collisionBoxCircle(this, _pCollider);
			}
		}
		else if (m_eColliderType == Collider_Circle)
		{
			if (_pCollider->m_eColliderType == Collider_Box)
			{
				m_bIsColliding = collisionBoxCircle(_pCollider, this);
			}
			else if (_pCollider->m_eColliderType == Collider_Circle)
			{
				m_bIsColliding = collisionCircleCircle(_pCollider, this);
			}
		}

		_pCollider->m_bIsColliding = m_bIsColliding;

		return m_bIsColliding;
	}

	Collider* Collider::loadFromFileJSON(string& _filename)
	{
		Json::Value root;
		std::ifstream config_doc(_filename, std::ifstream::binary);
		config_doc >> root;

		string szType = root["type"].asString();
		if (szType == "Circle")
		{
			CircleCollider* pCircleCollider = new CircleCollider;
			pCircleCollider->getCenter() = Vector2f(root["x"].asFloat(), root["y"].asFloat());
			pCircleCollider->getRadius() = root["radius"].asFloat();
			return pCircleCollider;
		}
		else if (szType == "Box")
		{
			BoxCollider* pBoxCollider = new BoxCollider;
			pBoxCollider->getOrigin() = Vector2f(root["x"].asFloat(), root["y"].asFloat());
			pBoxCollider->getSize() = Vector2f(root["w"].asFloat(), root["h"].asFloat());
			return pBoxCollider;
		}
		
		return nullptr;
	}

	bool Collider::init()
	{
		return true;
	}

	bool Collider::update()
	{
		return true;
	}

	bool Collider::draw()
	{
		return true;
	}

	bool Collider::quit()
	{
		return true;
	}

	bool Collider::collisionBoxBox(Collider* _pCollider1, Collider* _pCollider2)
	{
		Entity* pEntity1 = _pCollider1->getEntity();
		Entity* pEntity2 = _pCollider2->getEntity();
		Vector2f vPos1 = pEntity1 ? pEntity1->getPosition() : Vector2f(0.f, 0.f);
		Vector2f vPos2 = pEntity2 ? pEntity2->getPosition() : Vector2f(0.f, 0.f);
		BoxCollider* pBox1 = (BoxCollider*)_pCollider1;
		BoxCollider* pBox2 = (BoxCollider*)_pCollider2;
		
		Vector2f vWorldMin1 = vPos1 + pBox1->getMin();
		Vector2f vWorldMax1 = vPos1 + pBox1->getMax();
		Vector2f vWorldMin2 = vPos2 + pBox2->getMin();
		Vector2f vWorldMax2 = vPos2 + pBox2->getMax();

		for (char i = 0; i < 2; i++)
		{
			if (vWorldMax1.get(i) < vWorldMin2.get(i))
			{
				return false;
			}
			else if (vWorldMin1.get(i) > vWorldMax2.get(i))
			{
				return false;
			}
		}
		return true;
	}

	bool Collider::collisionBoxCircle(Collider* _pCollider1, Collider* _pCollider2)
	{
		Entity* pEntity1 = _pCollider1->getEntity();
		Entity* pEntity2 = _pCollider2->getEntity();
		Vector2f vPos1 = pEntity1 ? pEntity1->getPosition() : Vector2f(0.f, 0.f);
		Vector2f vPos2 = pEntity2 ? pEntity2->getPosition() : Vector2f(0.f, 0.f);
		BoxCollider* pBox1 = (BoxCollider*)_pCollider1;
		CircleCollider* pCircle2 = (CircleCollider*)_pCollider2;

		Vector2f vWorldMin1 = vPos1 + pBox1->getMin();
		Vector2f vWorldMax1 = vPos1 + pBox1->getMax();
		Vector2f vWorldCenter2 = vPos2 + pCircle2->getCenter();
		float fRadius = pCircle2->getRadius();

		float s, d = 0;
		//find the square of the distance
		//from the sphere to the box
		for (char i = 0; i<2; i++)
		{
			if (vWorldCenter2.get(i) < vWorldMin1.get(i))
			{
				s = vWorldCenter2.get(i) - vWorldMin1.get(i);
				d += s*s;
			}
			else if (vWorldCenter2.get(i) > vWorldMax1.get(i))
			{
				s = vWorldCenter2.get(i) - vWorldMax1.get(i);
				d += s*s;
			}
		}
		return d <= (fRadius*fRadius);
	}

	bool Collider::collisionCircleCircle(Collider* _pCollider1, Collider* _pCollider2)
	{
		Entity* pEntity1 = _pCollider1->getEntity();
		Entity* pEntity2 = _pCollider2->getEntity();
		Vector2f vPos1 = pEntity1 ? pEntity1->getPosition() : Vector2f(0.f, 0.f);
		Vector2f vPos2 = pEntity2 ? pEntity2->getPosition() : Vector2f(0.f, 0.f);
		CircleCollider* pCircle1 = (CircleCollider*)_pCollider1;
		CircleCollider* pCircle2 = (CircleCollider*)_pCollider2;

		Vector2f vWorldCenter1 = vPos1 + pCircle1->getCenter();
		Vector2f vWorldCenter2 = vPos2 + pCircle2->getCenter();
		float fRadius1 = pCircle1->getRadius();
		float fRadius2 = pCircle2->getRadius();

		float fDistSq = vWorldCenter1.distSq(vWorldCenter2);
		float fRadiuses = fRadius1 + fRadius2;
		if (fDistSq < (fRadiuses*fRadiuses))
		{
			return true;
		}
		return false;
	}

	BoxCollider::BoxCollider()
		: Collider(Collider_Box)
	{
	}

	BoxCollider::~BoxCollider()
	{
	}
	
	CircleCollider::CircleCollider()
		: Collider(Collider_Circle), m_fRadius(0.f)
	{
	}

	CircleCollider::~CircleCollider()
	{
	}

	Component* CircleCollider::clone()
	{
		Collider* pCollider = new CircleCollider(*this);
		// CB: Patch, make sure the circleCollider is dynamic...
		string s = this->m_szName + to_string(EntityManager::getSingleton()->getNewObjectID());
		PhysicsManager::getSingleton()->addDynamicCollider(s, pCollider);
		return pCollider;
	}
} // namespace crea
