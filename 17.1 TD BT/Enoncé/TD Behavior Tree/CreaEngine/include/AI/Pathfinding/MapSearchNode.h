// Definitions
#ifndef MAPSEARCHNODE_H
#define MAPSEARCHNODE_H

#include "AStarSearch.h"
namespace crea
{
	class CREAENGINE_API MapSearchNode
	{
	public:
		unsigned int x;	 // the (x,y) positions of the node
		unsigned int y;

		MapSearchNode(int _x = 0, int _y = 0) { x = _x; y = _y; }
		//~MapSearchNode() { } // CB: do not surdefine dest as it crashes with templates (why?) 

		float GoalDistanceEstimate(MapSearchNode &nodeGoal);
		bool IsGoal(MapSearchNode &nodeGoal);
		bool GetSuccessors(AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node);
		float GetCost(MapSearchNode &successor);
		bool IsSameState(MapSearchNode &rhs);

	};

}

#endif