#include "stdafx.h"

#include "Core\PhysicsManager.h"
#include "Physics\Collider.h"

namespace crea
{
	PhysicsManager::PhysicsManager()
	{
		m_pCurrentMap = nullptr;
	}

	PhysicsManager::~PhysicsManager()
	{
		clear();
	}

	PhysicsManager* PhysicsManager::getSingleton()
	{
		static PhysicsManager instanceUnique;
		return
			&instanceUnique;
	}

	void PhysicsManager::addDynamicCollider(string& _szName, Collider* _pCollider)
	{
		_pCollider->setName(_szName);
		m_DynamicColliders[_szName] = _pCollider;
	}

	void PhysicsManager::addStaticCollider(string& _szName, Collider* _pCollider)
	{
		_pCollider->setName(_szName);
		m_StaticColliders[_szName] = _pCollider;
	}

	bool PhysicsManager::isColliding(Collider* _pCollider, bool _bWithStatic, bool _bWithDynamic, bool _bWithTrigger)
	{
		bool bIsColliding = false;

		// With static collisions
		if (_bWithStatic)
		{
			MapStringCollider::iterator it = m_StaticColliders.begin();
			while (it != m_StaticColliders.end())
			{
				Collider* pCollider = (Collider*)(it->second);
				if (pCollider->isColliding(_pCollider, _bWithTrigger))
				{
					updateCollision(_pCollider, pCollider);
					bIsColliding = true;
				}
				++it;
			}
		}

		// With dynamic collisions
		if (_bWithStatic)
		{

		}

		// Cleanup old collisions
		cleanupCollisions();

		return bIsColliding;
	}

	Collider* PhysicsManager::getDynamicCollider(string& _szName, bool _bCloned)
	{
		MapStringCollider::iterator it = m_DynamicColliders.find(_szName);
		if (it == m_DynamicColliders.end())
		{
			Collider* pCollider = Collider::loadFromFileJSON(DATAAGENTPATH + _szName); // Create a default Collider if none exist

			if (!pCollider)
			{
				cerr << "Unable to open Collider file" << endl;
				return nullptr;
			}

			m_DynamicColliders[_szName] = pCollider;
			return pCollider;
		}
		else
		{
			if (_bCloned)
			{
				//return new Collider(it->second); // CB is it useful to clone?
			}
			else
			{
				return it->second;
			}
		}
		return nullptr;
	}

	Collider* PhysicsManager::getStaticCollider(string& _szName, bool _bCloned)
	{
		MapStringCollider::iterator it = m_StaticColliders.find(_szName);
		if (it == m_StaticColliders.end())
		{
			Collider* pCollider = Collider::loadFromFileJSON(DATAAGENTPATH + _szName); // Create a default Collider if none exist

			if (!pCollider)
			{
				cerr << "Unable to open Collider file" << endl;
				return nullptr;
			}

			m_StaticColliders[_szName] = pCollider;
			return pCollider;
		}
		else
		{
			if (_bCloned)
			{
				//return new Collider(it->second); // CB is it useful to clone?
			}
			else
			{
				return it->second;
			}
		}
		return nullptr;
	}

	void PhysicsManager::setCurrentMap(Map* _pMap)
	{
		m_pCurrentMap = _pMap;
	}

	Map* PhysicsManager::getCurrentMap()
	{
		return m_pCurrentMap;
	}

	bool PhysicsManager::onEnterCollision(Collider* _pCollider, Collider* _pOtherCollider)
	{
		Collision2D* pCollision = new Collision2D();
		pCollision->m_pCollider = _pCollider;
		pCollision->m_pOtherCollider = _pOtherCollider;
		pCollision->m_bUpdated = true;
		m_Collisions2D.insert(MapColliderCollision2D::value_type(_pCollider, pCollision));

		// Get a list of all scripts associated
		list<Script*> scripts;
		_pCollider->getEntity()->getComponents<Script>(scripts);
		// and call onCollisionEnter2D
		for (list<Script*>::iterator it = scripts.begin(); it != scripts.end(); ++it)
		{
			Script* pScript = (Script*)*it;
			pScript->onCollisionEnter2D(*pCollision);
		}

		return true;
	}
		
		
	bool PhysicsManager::updateCollision(Collider* _pCollider, Collider* _pOtherCollider)
	{
		// CB: todo: use of a multimap is not so nice (2 places to call new Collision2D) 
		// CB: also need to add onCollisionStay2D and onCollisionExit2D
		MapColliderCollision2D::iterator it = m_Collisions2D.find(_pCollider);
		if (it == m_Collisions2D.end())
		{
			// no collision with this collider yet
			onEnterCollision(_pCollider, _pOtherCollider);
		}
		else
		{
			long count = m_Collisions2D.count(_pCollider);
			for (long i = 0; i < count; ++i, ++it)
			{
				Collision2D* pCollision = (Collision2D*)it->second;
				if (pCollision->m_pOtherCollider == _pOtherCollider)
				{
					pCollision->m_bUpdated = true;
					return true;
				}
			}
			// no collision with this pair yet...
			onEnterCollision(_pCollider, _pOtherCollider);
		}
		return true;
	}

	bool PhysicsManager::cleanupCollisions()
	{
		long count = m_Collisions2D.size();
		MapColliderCollision2D::iterator it = m_Collisions2D.begin();
		for (long i = 0; i < count; ++i)
		{
			Collision2D* pCollision = (Collision2D*)it->second;
			if (pCollision->m_bUpdated == true)
			{
				pCollision->m_bUpdated = false;
				++it;
			}
			else
			{
				delete (*it).second;
				count--;
				it = m_Collisions2D.erase(it);
			}
		}
		return true;
	}

	bool PhysicsManager::init()
	{
		return true;
	}

	bool PhysicsManager::update()
	{
		// Check dynamic-static collisions
		MapStringCollider::iterator it = m_DynamicColliders.begin();
		while (it != m_DynamicColliders.end())
		{
			Collider* pCollider = (Collider*)(it->second);
			if (isColliding(pCollider))
			{
				// resolution

				return true;
			}
			++it;
		}

		return true;
	}

	bool PhysicsManager::draw()
	{
		return true;
	}

	void PhysicsManager::clear()
	{
		// Static
		MapStringCollider::iterator itStatic = m_StaticColliders.begin();
		while (itStatic != m_StaticColliders.end()) {
			delete (itStatic->second);
			itStatic = m_StaticColliders.erase(itStatic);
		}
		// Dynamic
		MapStringCollider::iterator itDynamic = m_DynamicColliders.begin();
		while (itDynamic != m_DynamicColliders.end()) {
			delete (itDynamic->second);
			itDynamic = m_DynamicColliders.erase(itDynamic);
		}

		m_pCurrentMap = nullptr;
	}

} // namespace crea
