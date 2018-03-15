#include "stdafx.h"
#include "AI\PathFinding\ClusterSearchNode.h"
#include "AI\PathFinding\ClusterSearchManager.h"
#include "Data\Map.h"
#include "Data\Node.h"
#include <math.h>

namespace crea
{
	bool ClusterSearchNode::IsSameState(ClusterSearchNode &rhs)
	{
		// same state in a search is simply when (x,y) are the same
		return ((x == rhs.x) && (y == rhs.y));
	}

	// Here's the heuristic function that estimates the distance from a Node to the Goal. 
	float ClusterSearchNode::GoalDistanceEstimate(ClusterSearchNode &nodeGoal)
	{
		float xd = float(((float)x - (float)nodeGoal.x));
		float yd = float(((float)y - (float)nodeGoal.y));

		//return sqrt((xd*xd) + (yd*yd));	// Pythagore
		return ((xd*xd) + (yd*yd));			// Simple Pythagore
		//return (abs(xd) + abs(yd));		// Manhattan
		//return max(abs(xd), abs(yd));		// Diagonal
	}

	bool ClusterSearchNode::IsGoal(ClusterSearchNode &nodeGoal)
	{
		return ((x == nodeGoal.x) && (y == nodeGoal.y));
	}

	// This generates the successors to the given Node. It uses a helper function called
	// AddSuccessor to give the successors to the AStar class. The A* specific initialisation
	// is done for each node internally, so here you just set the state information that
	// is specific to the application
	bool ClusterSearchNode::GetSuccessors(AStarSearch<ClusterSearchNode> *astarsearch, ClusterSearchNode *parent_node)
	{
		int parent_x = -1;
		int parent_y = -1;

		if (parent_node)
		{
			parent_x = parent_node->x;
			parent_y = parent_node->y;
		}

		ClusterSearchNode NewNode;
		Map* pMap = ClusterSearchManager::getSingleton()->getCurrentMap();
		Cluster* pCluster = ClusterSearchManager::getSingleton()->getCurrentCluster();

		// push each possible move except allowing the search to go backwards

		Node* pLeft = pCluster->isInCluster(x - 1, y) ? pMap->getNode(x - 1, y) : nullptr;
		if (pLeft && (pLeft->getTileCollisionId() == 0)
			&& !((parent_x == x - 1) && (parent_y == y))
			)
		{
			NewNode = ClusterSearchNode(x - 1, y);
			astarsearch->AddSuccessor(NewNode);
		}

		Node* pUp = pCluster->isInCluster(x, y - 1) ? pMap->getNode(x, y - 1) : nullptr;
		if (pUp && (pUp->getTileCollisionId() == 0)
			&& !((parent_x == x) && (parent_y == y - 1))
			)
		{
			NewNode = ClusterSearchNode(x, y - 1);
			astarsearch->AddSuccessor(NewNode);
		}

		Node* pRight = pCluster->isInCluster(x + 1, y) ? pMap->getNode(x + 1, y) : nullptr;
		if (pRight && (pRight->getTileCollisionId() == 0)
			&& !((parent_x == x + 1) && (parent_y == y))
			)
		{
			NewNode = ClusterSearchNode(x + 1, y);
			astarsearch->AddSuccessor(NewNode);
		}
		
		Node* pDown = pCluster->isInCluster(x, y + 1) ? pMap->getNode(x, y + 1) : nullptr;
		if (pDown && (pDown->getTileCollisionId() == 0)
			&& !((parent_x == x) && (parent_y == y + 1))
			)
		{
			NewNode = ClusterSearchNode(x, y + 1);
			astarsearch->AddSuccessor(NewNode);
		}
		
		// Diagonals
		Node* pUpLeft = pCluster->isInCluster(x - 1, y - 1) ? pMap->getNode(x - 1, y - 1) : nullptr;
		if (pUpLeft && (pUpLeft->getTileCollisionId() == 0)
			&& !((parent_x == x - 1) && (parent_y == y - 1))
			&& pUp && (pUp->getTileCollisionId() == 0)
			&& pLeft && (pLeft->getTileCollisionId() == 0)
			)
		{
			NewNode = ClusterSearchNode(x - 1, y - 1);
			astarsearch->AddSuccessor(NewNode);
		}

		Node* pDownRight = pCluster->isInCluster(x + 1, y + 1) ? pMap->getNode(x + 1, y + 1) : nullptr;
		if (pDownRight && (pDownRight->getTileCollisionId() == 0)
			&& !((parent_x == x + 1) && (parent_y == y + 1))
			&& pDown && (pDown->getTileCollisionId() == 0)
			&& pRight && (pRight->getTileCollisionId() == 0)
			)
		{
			NewNode = ClusterSearchNode(x + 1, y + 1);
			astarsearch->AddSuccessor(NewNode);
		}

		Node* pDownLeft = pCluster->isInCluster(x - 1, y + 1) ? pMap->getNode(x - 1, y + 1) : nullptr;
		if (pDownLeft && (pMap->getNode(x - 1, y + 1)->getTileCollisionId() == 0)
			&& !((parent_x == x - 1) && (parent_y == y + 1))
			&& pDown && (pDown->getTileCollisionId() == 0)
			&& pLeft && (pLeft->getTileCollisionId() == 0)
			)
		{
			NewNode = ClusterSearchNode(x - 1, y + 1);
			astarsearch->AddSuccessor(NewNode);
		}

		Node* pUpRight = pCluster->isInCluster(x + 1, y - 1) ? pMap->getNode(x + 1, y - 1) : nullptr;
		if (pUpRight && (pMap->getNode(x + 1, y - 1)->getTileCollisionId() == 0)
			&& !((parent_x == x + 1) && (parent_y == y - 1))
			&& pUp && (pUp->getTileCollisionId() == 0)
			&& pRight && (pRight->getTileCollisionId() == 0)
			)
		{
			NewNode = ClusterSearchNode(x + 1, y - 1);
			astarsearch->AddSuccessor(NewNode);
		}
		
		return true;
	}

	// given this node, what does it cost to move to successor. 
	float ClusterSearchNode::GetCost(ClusterSearchNode &successor)
	{
		// The G cost is 1.4 for diagonal move and 1.0 for horizontal or vertical move
		int dx = successor.x - x;
		int dy = successor.y - y;
		if ((dx != 0) && (dy != 0))
		{
			return 1.4f;
		}
		else if ((dx != 0) && (dy == 0))
		{
			return 1.0f;
		}
		else if ((dx == 0) && (dy != 0))
		{
			return 1.0f;
		}

		return 0.0f;
	}

}