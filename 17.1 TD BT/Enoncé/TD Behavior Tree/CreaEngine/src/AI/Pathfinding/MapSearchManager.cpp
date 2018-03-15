#include "stdafx.h"

#include "AI\Pathfinding\MapSearchManager.h"

namespace crea
{
	MapSearchManager::MapSearchManager()
	{
		m_pMap = nullptr;
	}

	MapSearchManager::~MapSearchManager()
	{
	}	

	MapSearch* MapSearchManager::getMapSearch(string _szName)
	{
		MapStringMapSearch::iterator it = m_MapSearches.find(_szName);
		if (it == m_MapSearches.end())
		{
			MapSearch* pMapSearch = new MapSearch();
			m_MapSearches[_szName] = pMapSearch;
			return pMapSearch;
		}
		else
		{
			return it->second;
		}
	}

}