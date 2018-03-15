/***************************************************/
/* Nom:	MapSearch.h
/* Description: MapSearch
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __MapSearch_H__
#define __MapSearch_H__

#include "AI\AI.h"
#include "AI\Pathfinding\AStarSearch.h"
#include "AI\Pathfinding\MapSearchNode.h"

namespace crea
{
	class CREAENGINE_API MapSearch
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

		MapSearch();
		~MapSearch();

		bool setStartAndGoal(Vector2f _vStart, Vector2f _vGoal, bool _bFromPixels = true);
		SearchState update();
		bool getSolution(VectorVector2f& _vSolution);
		float getSolutionLength();

	private:
		// A*
		AStarSearch<MapSearchNode> m_pAStarSearch;
		SearchState m_uiSearchState;

	};

}
#endif	// __MapSearch_H__