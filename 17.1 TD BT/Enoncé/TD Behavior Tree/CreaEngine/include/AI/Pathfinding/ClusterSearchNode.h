/***************************************************/
/* Nom:	ClusterSearchNode.h
/* Description: ClusterSearchNode
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef ClusterSearchNode_H
#define ClusterSearchNode_H

#include "AStarSearch.h"
namespace crea
{
	class CREAENGINE_API ClusterSearchNode // Cluster
	{
	public:
		unsigned int x;	 // the (x,y) positions of the node
		unsigned int y;

		ClusterSearchNode(int _x = 0, int _y = 0) { x = _x; y = _y; }
		//~ClusterSearchNode() { } // CB: do not surdefine dest as it crashes with templates (why?) 

		float GoalDistanceEstimate(ClusterSearchNode &nodeGoal);
		bool IsGoal(ClusterSearchNode &nodeGoal);
		bool GetSuccessors(AStarSearch<ClusterSearchNode> *astarsearch, ClusterSearchNode *parent_node);
		float GetCost(ClusterSearchNode &successor);
		bool IsSameState(ClusterSearchNode &rhs);

	};

}

#endif