/***************************************************/
/* Nom:	ClusterSearch.h
/* Description: ClusterSearch
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __ClusterSearch_H__
#define __ClusterSearch_H__

#include "AI\AI.h"
#include "AI\Pathfinding\AStarSearch.h"
#include "AI\Pathfinding\ClusterSearchNode.h"

namespace crea
{
	class CREAENGINE_API ClusterSearch
	{
	public:
		enum SearchState
		{
			SEARCH_STATE_NOT_INITIALISED,
			SEARCH_STATE_SEARCHING,
			SEARCH_STATE_SUCCEEDED,
			SEARCH_STATE_FAILED,
			SEARCH_STATE_OUT_OF_MEMORY,
			SEARCH_STATE_INVALID
		};

		ClusterSearch();
		~ClusterSearch();

		bool setStartAndGoal(Vector2f _vStart, Vector2f _vGoal, bool _bFromPixels = true);
		SearchState update();
		bool getSolution(VectorVector2f& _vSolution);
		float getSolutionLength();

	private:
		// A*
		AStarSearch<ClusterSearchNode> m_pAStarSearch;
		SearchState m_uiSearchState;

	};

}
#endif	// __ClusterSearch_H__