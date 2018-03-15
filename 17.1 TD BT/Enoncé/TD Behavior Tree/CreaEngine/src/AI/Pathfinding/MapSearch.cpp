#include "stdafx.h"

#include "AI\Pathfinding\MapSearch.h"

namespace crea
{
	MapSearch::MapSearch()
	{
		m_uiSearchState = (SearchState) AStarSearch<MapSearchNode>::SEARCH_STATE_NOT_INITIALISED;
	}

	MapSearch::~MapSearch()
	{
	}

	bool MapSearch::setStartAndGoal(Vector2f _vStart, Vector2f _vGoal, bool _bFromPixels)
	{
		bool bAvailable = false;
		if (m_uiSearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_NOT_INITIALISED)
		{
			bAvailable = true;
		}
		if (m_uiSearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED)
		{
			bAvailable = true;
		}
		if (m_uiSearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED)
		{
			m_pAStarSearch.FreeSolutionNodes();
			bAvailable = true;
		}

		if (bAvailable)
		{
			Map* pMap = MapSearchManager::getSingleton()->getCurrentMap();
			if (_bFromPixels)
			{
				_vStart = pMap->getNodePositionFromPixels(_vStart);
				_vGoal = pMap->getNodePositionFromPixels(_vGoal);
			}

			// CB: we should first make sure that the start and goal is accessible, otherwise there is no solution, no need to search!

			MapSearchNode nodeStart((int)_vStart.getX(), (int)_vStart.getY());
			MapSearchNode nodeEnd((int)_vGoal.getX(), (int)_vGoal.getY());
			// Set Start and goal states	
			m_pAStarSearch.SetStartAndGoalStates(nodeStart, nodeEnd);
			m_uiSearchState = (SearchState)m_pAStarSearch.SearchStep();
		}

		return bAvailable;
	}

	MapSearch::SearchState MapSearch::update()
	{
		if (m_uiSearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING)
		{
			// Step into search
			m_uiSearchState = (SearchState) m_pAStarSearch.SearchStep();
		}
		return m_uiSearchState;
	}

	bool MapSearch::getSolution(VectorVector2f& _vSolution)
	{
		if (m_uiSearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED)
		{
			Map* pMap = MapSearchManager::getSingleton()->getCurrentMap();
			MapSearchNode* node = m_pAStarSearch.GetSolutionStart();
			while (node)
			{
				Vector2f vNode = pMap->getPixelsFromNodePosition(Vector2f((float)node->x, (float)node->y));
				_vSolution.push_back(new Vector2f(vNode));
				node = m_pAStarSearch.GetSolutionNext();
			}
			return true;
		}
		return false;
	}

	float MapSearch::getSolutionLength()
	{
		if (m_uiSearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED)
		{
			float fDist = 0.0f;
			Map* pMap = MapSearchManager::getSingleton()->getCurrentMap();
			MapSearchNode* node1 = m_pAStarSearch.GetSolutionStart();
			MapSearchNode* node2 = m_pAStarSearch.GetSolutionNext();
			while (node1 && node2)
			{
				fDist += node1->GetCost(*node2);
				node1 = node2;
				node2 = m_pAStarSearch.GetSolutionNext();
			}
			return fDist;
		}
		return 0.f;
	}
}