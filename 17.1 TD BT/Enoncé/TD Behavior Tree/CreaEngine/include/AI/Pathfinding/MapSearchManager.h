/***************************************************/
/* Nom:	MapSearchManager.h
/* Description: MapSearchManager
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __MapSearchManager_H__
#define __MapSearchManager_H__

#include "AI\AI.h"
#include "AI\Pathfinding\MapSearch.h"

namespace crea
{
	class CREAENGINE_API MapStringMapSearch : public map<string, MapSearch*> {};

	class CREAENGINE_API MapSearchManager : public Singleton <MapSearchManager>
	{
	public:

		MapSearchManager();
		~MapSearchManager();

		MapSearch* getMapSearch(string _szName);

		// A*
		Map* getCurrentMap() { return m_pMap; }
		void setCurrentMap(Map* _pMap) { m_pMap = _pMap;  }

		// AA*
		Agent* getCurrentAgent() { return m_pAgent; }
		void setCurrentAgent(Agent* _pAgent) { m_pAgent = _pAgent; }

	private:

		MapStringMapSearch m_MapSearches;
		Map* m_pMap;
		Agent* m_pAgent;
	};

}
#endif	// __MapSearchManager_H__