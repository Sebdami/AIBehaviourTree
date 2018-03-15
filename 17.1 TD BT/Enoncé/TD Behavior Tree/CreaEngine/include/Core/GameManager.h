/***************************************************/
/* Nom:	GameManager.h
/* Description: GameManager
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __GameManager_H_
#define __GameManager_H_

#include "SceneManager.h"
#include "EntityManager.h"
#include "Core\DataManager.h"
#include "Input\InputManager.h"
#include "Core\Math.h"
#include "AI\Agent.h"
#include "AI\Pathfinding\MapSearchManager.h"
#include "AI\Pathfinding\ClusterSearchManager.h"
#include "AI\Steering\Steering.h"
#include "Core\Script.h"
#include "Core\PhysicsManager.h"

namespace sf
{
	class RenderWindow;
}

namespace crea
{
	class Collider;

	class CREAENGINE_API GameManager
	{
		GameManager();

		// window
		sf::RenderWindow* m_pWindow;
		IntRect m_rWindowRect;

	public:
		~GameManager();

		static GameManager* getSingleton();

		inline sf::RenderWindow* getWindow() { return m_pWindow; }

		IntRect getWindowRect() { return m_rWindowRect; }

		void setWindowRect(IntRect _rect) {m_rWindowRect = _rect; }

		inline void setScene(Scene* s) { SceneManager::getSingleton()->setScene(s); }

		// Input
		inline bool isKeyPressed(Key _key) { return InputManager::getSingleton()->isKeyPressed(_key); }

		inline bool isMouseButtonPressed(Button _button) { return InputManager::getSingleton()->isMouseButtonPressed(_button); }

		inline Vector2f getMousePosition() { return InputManager::getSingleton()->getMousePosition(); }

		// Data
		inline Font* getFont(string _szName) { return DataManager::getSingleton()->getFont(_szName); }

		inline Texture* getTexture(string _szName) { return DataManager::getSingleton()->getTexture(_szName); }

		inline Color* getColor(string _szName) { return DataManager::getSingleton()->getColor(_szName); }

		inline Text* getText(string _szName) { return DataManager::getSingleton()->getText(_szName); }

		inline Sprite* getSprite(string _szName) { return DataManager::getSingleton()->getSprite(_szName); }

		inline Map* getMap(string _szName) { return DataManager::getSingleton()->getMap(_szName); }

		inline Agent* getAgent(string _szName) { return DataManager::getSingleton()->getAgent(_szName); }

		inline Animation* getAnimation(string _szName) { return DataManager::getSingleton()->getAnimation(_szName); }

		inline ActionTable* getActionTable(string _szName) { return DataManager::getSingleton()->getActionTable(_szName); }

		inline Vehicle* getVehicle(string _szName) { return DataManager::getSingleton()->getVehicle(_szName); }

		inline Animator* getAnimator(string _szName) { return EntityManager::getSingleton()->getAnimator(_szName); }

		inline Script* getScript(string _szName) { return EntityManager::getSingleton()->getScript(_szName); }

		inline Steering* getSteering(string _szName) { return EntityManager::getSingleton()->getSteering(_szName); }

		inline void setScriptFactory(ScriptFactory* _pScriptFactory) { return EntityManager::getSingleton()->setScriptFactory(_pScriptFactory); }

		inline MapRenderer* getMapRenderer(string _szName) { return EntityManager::getSingleton()->getMapRenderer(_szName); }

		inline TextRenderer* getTextRenderer(string _szName) { return EntityManager::getSingleton()->getTextRenderer(_szName); }

		inline SpriteRenderer* getSpriteRenderer(string _szName) { return EntityManager::getSingleton()->getSpriteRenderer(_szName); }

		inline Entity* getEntity(string _szName) { return EntityManager::getSingleton()->getEntity(_szName); }

		inline Entity* instanciate(string _szName, Entity* _pEntity) { return EntityManager::getSingleton()->instanciate(_szName, _pEntity); }

		inline void addEntity(Entity* _pEntity, Entity* _pParent = nullptr)  { EntityManager::getSingleton()->addEntity(_pEntity, _pParent); }

		inline Collider* getDynamicCollider(string _szName) { return PhysicsManager::getSingleton()->getDynamicCollider(_szName); }

		inline Collider* getStaticCollider(string _szName) { return PhysicsManager::getSingleton()->getStaticCollider(_szName); }

		inline MapSearch* getMapSearch(string _szName) { return MapSearchManager::getSingleton()->getMapSearch(_szName); }

		inline ClusterSearch* getClusterSearch(string _szName) { return ClusterSearchManager::getSingleton()->getClusterSearch(_szName); }

		inline void clearAllData() { DataManager::getSingleton()->clear(); }

		inline void clearAllEntities()  { EntityManager::getSingleton()->clear(); }

		inline void clearEntity(Entity* _pEntity) { EntityManager::getSingleton()->clearEntity(_pEntity); }

		// Selection
		inline void selectEntities(Vector2f _vStart, Vector2f _vEnd) { EntityManager::getSingleton()->selectEntities(_vStart, _vEnd); }

		inline void unselectEntities() { EntityManager::getSingleton()->unselectEntities(); }

		inline ListEntity* getSelectedEntities() { return EntityManager::getSingleton()->getSelectedEntities(); }

		void init();

		void update();

		void quit();
	};

} // namespace crea

#endif
