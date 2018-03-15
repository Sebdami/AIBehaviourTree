#include "stdafx.h"

#include "Core\EntityManager.h"
#include "Core\Script.h"
#include "Data\Entity.h"

namespace crea
{
	EntityManager::EntityManager()
	{
		m_pRoot = new Entity();
		m_pRoot->setName(string("root"));
		m_pScriptFactory = nullptr;
		nextFreeID = 1;
	}

	EntityManager::~EntityManager()
	{
		clear();
		delete m_pRoot;
		m_pRoot = nullptr;
	}

	EntityManager* EntityManager::getSingleton()
	{
		static EntityManager instanceUnique;
		return
			&instanceUnique;
	}

	Entity* EntityManager::getEntity(string& _szName)
	{
		Entity* pEntity = nullptr;
		pEntity = m_pRoot->getEntity(_szName);
		if (!pEntity)
		{
			pEntity = new Entity();
			pEntity->setName(_szName);
			pEntity->SetID(getNewObjectID());
			Store(*pEntity);
		}
		return pEntity;
	}
	
	Entity* EntityManager::instanciate(string& _szName, Entity* _pEntity)
	{
		Entity* pEntity = nullptr;
		pEntity = m_pRoot->getEntity(_szName);
		if (!pEntity)
		{
			pEntity = _pEntity->clone();
			pEntity->init();
			pEntity->setName(_szName);
			pEntity->SetID(getNewObjectID());
			Store(*pEntity);
		}
		else
		{
			cerr << "Name: " << _szName << " already exists, can't instanciate Entity." << endl;
		}
		return pEntity;

	}

	void EntityManager::addEntity(Entity* _pEntity, Entity* _pParent)
	{
		if (_pParent)
		{
			_pParent->addChild(_pEntity);
		}
		else
		{
			m_pRoot->addChild(_pEntity);
		}
	}

	TextRenderer* EntityManager::getTextRenderer(string _szName, bool _bCloned)
	{
		MapStringTextRenderer::iterator it = m_pTextRenderers.find(_szName);
		if (it == m_pTextRenderers.end())
		{
			TextRenderer* pTextRenderer = new TextRenderer(); // Create a default TextRenderer if none exist
			m_pTextRenderers[_szName] = pTextRenderer;
			return pTextRenderer;
		}
		else
		{
			if (_bCloned)
			{
				//return new TextRenderer(it->second); // CB is it useful to clone?
			}
			else
			{
				return it->second;
			}
		}
		return nullptr;
	}

	SpriteRenderer* EntityManager::getSpriteRenderer(string _szName, bool _bCloned)
	{
		MapStringSpriteRenderer::iterator it = m_pSpriteRenderers.find(_szName);
		if (it == m_pSpriteRenderers.end())
		{
			SpriteRenderer* pSpriteRenderer = new SpriteRenderer(); // Create a default SpriteRenderer if none exist
			m_pSpriteRenderers[_szName] = pSpriteRenderer;
			return pSpriteRenderer;
		}
		else
		{
			if (_bCloned)
			{
				//return new SpriteRenderer(it->second); // CB is it useful to clone?
			}
			else
			{
				return it->second;
			}
		}
		return nullptr;
	}

	MapRenderer* EntityManager::getMapRenderer(string _szName, bool _bCloned)
	{
		MapStringMapRenderer::iterator it = m_pMapRenderers.find(_szName);
		if (it == m_pMapRenderers.end())
		{
			MapRenderer* pMapRenderer = new MapRenderer(); // Create a default MapRenderer if none exist
			m_pMapRenderers[_szName] = pMapRenderer;
			return pMapRenderer;
		}
		else
		{
			if (_bCloned)
			{
				//return new MapRenderer(it->second); // CB is it useful to clone?
			}
			else
			{
				return it->second;
			}
		}
		return nullptr;
	}

	Animator* EntityManager::getAnimator(string _szName, bool _bCloned)
	{
		MapStringAnimator::iterator it = m_pAnimators.find(_szName);
		if (it == m_pAnimators.end())
		{
			Animator* pAnimator = new Animator(); // Create a default Animator if none exist
			m_pAnimators[_szName] = pAnimator;
			return pAnimator;
		}
		else
		{
			if (_bCloned)
			{
				//return new Animator(it->second); // CB is it useful to clone?
			}
			else
			{
				return it->second;
			}
		}
		return nullptr;
	}

	Script* EntityManager::getScript(string _szName, bool _bCloned)
	{
		MapStringScript::iterator it = m_pScripts.find(_szName);
		if (it == m_pScripts.end())
		{
			if (m_pScriptFactory)
			{
				Script* pScript = m_pScriptFactory->create(_szName); // Create a Script using the factory (should be set first using setScriptFactory)
				m_pScripts[_szName] = pScript;
				return pScript;
			}
		}
		else
		{
			if (_bCloned)
			{
				//return new Script(it->second); // CB is it useful to clone?
			}
			else
			{
				return it->second;
			}
		}
		return nullptr;
	}

	Steering* EntityManager::getSteering(string _szName, bool _bCloned)
	{
		MapStringSteering::iterator it = m_pSteerings.find(_szName);
		if (it == m_pSteerings.end())
		{
			Steering* pSteering = new Steering(); // Create a default Steering if none exist
			m_pSteerings[_szName] = pSteering;
			return pSteering;
		}
		else
		{
			if (_bCloned)
			{
				//return new Steering(it->second); // CB is it useful to clone?
			}
			else
			{
				return it->second;
			}
		}
		return nullptr;
	}

	void EntityManager::selectEntities(Vector2f _vStart, Vector2f _vEnd)
	{
		FloatRect rect(_vStart, _vEnd);
		m_pRoot->selectEntities(rect);
	}

	void EntityManager::unselectEntities()
	{
		m_pRoot->unselectEntities();
		m_pSelectedEntities.clear();
	}

	void EntityManager::addSelectedEntity(Entity* _pEntity)
	{
		m_pSelectedEntities.push_back(_pEntity);
	}

	void EntityManager::Store(Entity& _entity)
	{
		if (Find(_entity.GetID()) == 0) {
			m_Entities[_entity.GetID()] = &_entity;
		}
		else {
			assert(!"EntityManager::Store - Object ID already represented in database.");
		}
	}


	void EntityManager::Remove(objectID _id)
	{
		MapObjectIDEntity::iterator it = m_Entities.find(_id);
		if (it != m_Entities.end())
		{
			m_Entities.erase(it);
		}

		return;
	}


	Entity* EntityManager::Find(objectID _id)
	{
		MapObjectIDEntity::iterator it = m_Entities.find(_id);
		if (it != m_Entities.end())
		{
			return it->second;
		}

		return(0);

	}

	objectID EntityManager::getNewObjectID()
	{
		return(nextFreeID++);

	}

	void EntityManager::clearEntity(Entity* _pEntity)
	{
		m_pRoot->removeEntity(_pEntity);
		delete _pEntity;
	}

	bool EntityManager::init()
	{
		return m_pRoot->init();
	}

	bool EntityManager::update()
	{
		return m_pRoot->update();
	}

	bool EntityManager::draw()
	{		
		return m_pRoot->draw();
	}

	void EntityManager::clear()
	{
		MapStringTextRenderer::iterator itTextRenderer = m_pTextRenderers.begin();
		while (m_pTextRenderers.size()) {
			delete (*itTextRenderer).second;
			itTextRenderer = m_pTextRenderers.erase(itTextRenderer);
		}

		MapStringSpriteRenderer::iterator itSpriteRenderer = m_pSpriteRenderers.begin();
		while (m_pSpriteRenderers.size()) {
			delete (*itSpriteRenderer).second;
			itSpriteRenderer = m_pSpriteRenderers.erase(itSpriteRenderer);
		}

		MapStringMapRenderer::iterator itMapRenderer = m_pMapRenderers.begin();
		while (m_pMapRenderers.size()) {
			delete (*itMapRenderer).second;
			itMapRenderer = m_pMapRenderers.erase(itMapRenderer);
		}

		MapStringAnimator::iterator itAnimator = m_pAnimators.begin();
		while (m_pAnimators.size()) {
			delete (*itAnimator).second;
			itAnimator = m_pAnimators.erase(itAnimator);
		}

		MapStringScript::iterator itScript = m_pScripts.begin();
		while (m_pScripts.size()) {
			delete (*itScript).second;
			itScript = m_pScripts.erase(itScript);
		}

		MapStringSteering::iterator itSteering = m_pSteerings.begin();
		while (m_pSteerings.size()) {
			delete (*itSteering).second;
			itSteering = m_pSteerings.erase(itSteering);
		}

		if (m_pRoot)
		{
			m_pRoot->clear();
		}
	}
} // namespace crea
