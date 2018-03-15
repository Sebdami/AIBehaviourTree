/***************************************************/
/* Nom:	ClusterSearchManager.h
/* Description: ClusterSearchManager
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __ClusterSearchManager_H__
#define __ClusterSearchManager_H__

#include "AI\AI.h"
#include "AI\Pathfinding\ClusterSearch.h"

namespace crea
{
	class Cluster;
	class CREAENGINE_API MapStringClusterSearch : public map<string, ClusterSearch*> {};

	class CREAENGINE_API ClusterSearchManager : public Singleton <ClusterSearchManager>
	{
	public:

		ClusterSearchManager();
		~ClusterSearchManager();

		ClusterSearch* getClusterSearch(string _szName);

		Map* getCurrentMap() { return m_pMap; }
		void setCurrentMap(Map* _pMap) { m_pMap = _pMap;  }
		Cluster* getCurrentCluster() { return m_pCluster; }
		void setCurrentCluster(Cluster* _pCluster) { m_pCluster = _pCluster; }

	private:

		MapStringClusterSearch m_ClusterSearches;
		Map* m_pMap;
		Cluster* m_pCluster;
	};

}
#endif	// __ClusterSearchManager_H__