#include "stdafx.h"

#include "json\json.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "Data\Entity.h"
#include "Core\Component.h"
#include "AI\Agent.h"
#include "AI\ActionTable.h"
#include "Core\Script.h"
#include "Physics\Collider.h"

namespace crea
{
	Entity::Entity()
	{
		m_pParent = nullptr;
		m_szName = "Entity"; 
		m_ID = 0;
	}

	Entity::Entity(Entity& _entity)
	{
		m_pParent = nullptr;
		m_szName = _entity.m_szName + " Instance";
		m_ID = -1;

		// Components
		for (list<Component*>::iterator it = _entity.m_pComponents.begin(); it != _entity.m_pComponents.end(); ++it)
		{
			Component* p = (*it)->clone();
			p->setEntity(this); 
			addComponent(p);
		}

		// Children
		for (list<Entity*>::iterator it = _entity.m_pChildren.begin(); it != _entity.m_pChildren.end(); ++it)
		{
			addChild((*it)->clone());
		}
	}

	Entity::~Entity()
	{
		clear();
	}

	void Entity::addChild(Entity* _pEntity)
	{
		m_pChildren.push_back(_pEntity);
		_pEntity->setParent(this);
	}

	void Entity::removeChild(Entity* _pEntity)
	{
		_pEntity->setParent(nullptr);
		m_pChildren.remove(_pEntity);
	}

	void Entity::addComponent(Component* _pComponent)
	{
		_pComponent->setEntity(this);
		m_pComponents.push_back(_pComponent);
	}

	void Entity::removeComponent(Component* _pComponent)
	{
		_pComponent->setEntity(nullptr);
		auto a = std::remove_if(m_pComponents.begin(), m_pComponents.end(),
			[=](Component* p) { return p == _pComponent; });

		m_pComponents.erase(a);
	}

	bool Entity::init()
	{
		// Components
		for (list<Component*>::iterator it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
		{
			(*it)->init();
		}

		// Children
		for (list<Entity*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); ++it)
		{
			(*it)->init();
		}

		return true;
	}

	bool Entity::update()
	{
		// Components
		for (list<Component*>::iterator it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
		{
			(*it)->update();
		}

		// Children
		for (list<Entity*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); ++it)
		{
			(*it)->update();
		}

		return true;
	}

	bool Entity::draw()
	{
		// Components
		for (list<Component*>::iterator it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
		{
			(*it)->draw();
		}

		// Children
		for (list<Entity*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); ++it)
		{
			(*it)->draw();
		}


		return true;
	}

	void Entity::clear()
	{
		// Children
		for (list<Entity*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); ++it)
		{
			delete (*it);
		}
		m_pChildren.clear();

		// Components
		for (list<Component*>::iterator it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
		{
			//delete (*it);// CB: the components are destroyed by managers...
		}
		m_pComponents.clear();
	}

	Entity* Entity::getEntity(string& _szName)
	{
		if (hasName(_szName))
		{
			return this;
		}
		else
		{
			// Children
			for (list<Entity*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); ++it)
			{
				Entity* pEntity = (*it)->getEntity(_szName);
				if (pEntity)
				{
					return pEntity;
				}
			}
		}
		return nullptr;
	}

	Entity* Entity::getEntity(Entity* _pEntity)
	{
		if (_pEntity == this)
		{
			return this;
		}
		else
		{
			// Children
			for (list<Entity*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); ++it)
			{
				Entity* pEntity = (*it)->getEntity(_pEntity);
				if (pEntity)
				{
					return pEntity;
				}
			}
		}
		return nullptr;
	}

	bool Entity::removeEntity(Entity* _pEntity)
	{
		if (_pEntity->m_pParent != nullptr)
		{
			_pEntity->m_pParent->removeChild(_pEntity);
			return true;
		}
		return false;
	}

	void Entity::selectEntities(FloatRect& _rect)
	{
		if (_rect.contains(getPosition()))
		{
			m_bSelected = true;
			EntityManager::getSingleton()->addSelectedEntity(this);
		}
		else
		{
			// Children
			for (list<Entity*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); ++it)
			{
				(*it)->selectEntities(_rect);
			}
		}
	}

	void Entity::unselectEntities()
	{
		m_bSelected = false;

		// Children
		for (list<Entity*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); ++it)
		{
			(*it)->unselectEntities();
		}
	}

	bool Entity::loadFromFileJSON(string& _filename)
	{
		Json::Value root;
		std::ifstream entityStream(_filename, std::ifstream::binary);
		if (entityStream.fail())
		{
			cerr << "Can't open Entity file: " << _filename << endl;
			return false;
		}

		// Parse file
		entityStream >> root;

		crea::GameManager*	pGM = crea::GameManager::getSingleton();

		// Components
		Json::Value components = root["components"];
		for (unsigned int iComponent = 0; iComponent < components.size(); ++iComponent)
		{
			Json::Value component = components[iComponent];

			string szType = component["type"].asString();
			if (szType == "SpriteRenderer")
			{
				string szName = component["name"].asString();
				SpriteRenderer* pSpriteRenderer = pGM->getSpriteRenderer(szName);

				string szSprite = component["sprite"].asString();
				Sprite* pSprite = pGM->getSprite(szSprite);

				string szTexture = component["image"].asString();
				Texture* pTexture = pGM->getTexture(szTexture);
				pSprite->setTexture(pTexture);

				pSpriteRenderer->setSprite(pSprite);
				addComponent(pSpriteRenderer);
			}
			else if (szType == "Agent")
			{
				string szName = component["name"].asString();
				Agent* pAgent = pGM->getAgent(szName);

				addComponent(pAgent);
			}
			else if (szType == "Animator")
			{
				string szName = component["name"].asString();
				Animator* pAnimator = pGM->getAnimator(szName);

				string szSprite = component["sprite"].asString();
				Sprite* pSprite = pGM->getSprite(szSprite);

				pAnimator->setSprite(pSprite);
				addComponent(pAnimator);
			}
			else if (szType == "ActionTable")
			{
				string szName = component["name"].asString();
				ActionTable* pActionTable = pGM->getActionTable(szName);
				addComponent(pActionTable);
			}
			else if (szType == "Script")
			{
				string szName = component["name"].asString();
				Script* pScript = pGM->getScript(szName);
				addComponent(pScript);
			}
			else if (szType == "Collider")
			{
				string szName = component["name"].asString();
				Collider* pCollider = pGM->getDynamicCollider(szName);
				addComponent(pCollider);
			}
			else if (szType == "Steering")
			{
				string szName = component["name"].asString();
				Steering* pSteering = pGM->getSteering(szName);
				addComponent(pSteering);
			}
			else if (szType == "Vehicle")
			{
				string szName = component["name"].asString();
				Vehicle* pVehicle = pGM->getVehicle(szName);
				addComponent(pVehicle);
			}
		}
		return true;
	}

} // namespace crea
