#include "stdafx.h"
#include "AI\PathFinding\MapSearchNode.h"
#include "AI\PathFinding\MapSearchManager.h"
#include "Data\Map.h"
#include "Data\Node.h"
#include <math.h>

namespace crea
{
	bool MapSearchNode::IsSameState(MapSearchNode &rhs)
	{
		// same state in a search is simply when (x,y) are the same
		return ((x == rhs.x) && (y == rhs.y));
	}

	// Here's the heuristic function that estimates the distance from a Node to the Goal. 
	float MapSearchNode::GoalDistanceEstimate(MapSearchNode &nodeGoal)
	{
		float xd = float(((float)x - (float)nodeGoal.x));
		float yd = float(((float)y - (float)nodeGoal.y));

		//return sqrt((xd*xd) + (yd*yd));	// Pythagore
		return ((xd*xd) + (yd*yd));			// Simple Pythagore
		//return (abs(xd) + abs(yd));		// Manhattan
		//return max(abs(xd), abs(yd));		// Diagonal
	}

	bool MapSearchNode::IsGoal(MapSearchNode &nodeGoal)
	{
		return ((x == nodeGoal.x) && (y == nodeGoal.y));
	}

	// This generates the successors to the given Node. It uses a helper function called
	// AddSuccessor to give the successors to the AStar class. The A* specific initialisation
	// is done for each node internally, so here you just set the state information that
	// is specific to the application
	bool MapSearchNode::GetSuccessors(AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node)
	{
		int parent_x = -1;
		int parent_y = -1;

		if (parent_node)
		{
			parent_x = parent_node->x;
			parent_y = parent_node->y;
		}

		MapSearchNode NewNode;
		Map* pMap = MapSearchManager::getSingleton()->getCurrentMap();

		// AA*
		Agent* pAgent = nullptr;
		short nAgentSize = 0;
		pAgent = MapSearchManager::getSingleton()->getCurrentAgent();
		if (pAgent)
		{
			nAgentSize = pAgent->getSize();
		}

		// push each possible move except allowing the search to go backwards

		Node* pLeft = pMap->getNode(x - 1, y);
		if (pLeft && (pLeft->getTileCollisionId() == 0) 
			&& (pLeft->getTileClearanceId() >= nAgentSize)
			&& !((parent_x == x - 1) && (parent_y == y))
			)
		{
			NewNode = MapSearchNode(x - 1, y);
			astarsearch->AddSuccessor(NewNode);
		}

		Node* pUp = pMap->getNode(x, y - 1);
		if (pUp && (pUp->getTileCollisionId() == 0) 
			&& (pUp->getTileClearanceId() >= nAgentSize)
			&& !((parent_x == x) && (parent_y == y - 1))
			)
		{
			NewNode = MapSearchNode(x, y - 1);
			astarsearch->AddSuccessor(NewNode);
		}

		Node* pRight = pMap->getNode(x + 1, y);
		if (pRight && (pRight->getTileCollisionId() == 0) 
			&& (pRight->getTileClearanceId() >= nAgentSize)
			&& !((parent_x == x + 1) && (parent_y == y))
			)
		{
			NewNode = MapSearchNode(x + 1, y);
			astarsearch->AddSuccessor(NewNode);
		}
		
		Node* pDown = pMap->getNode(x, y + 1);
		if (pDown && (pDown->getTileCollisionId() == 0) 
			&& (pDown->getTileClearanceId() >= nAgentSize)
			&& !((parent_x == x) && (parent_y == y + 1))
			)
		{
			NewNode = MapSearchNode(x, y + 1);
			astarsearch->AddSuccessor(NewNode);
		}
		
		// Diagonals
		Node* pUpLeft = pMap->getNode(x - 1, y - 1);
		if (pUpLeft && (pUpLeft->getTileCollisionId() == 0) 
			&& (pUpLeft->getTileClearanceId() >= nAgentSize)
			&& !((parent_x == x - 1) && (parent_y == y - 1))
			&& pUp && (pUp->getTileCollisionId() == 0)
			&& pLeft && (pLeft->getTileCollisionId() == 0)
			)
		{
			NewNode = MapSearchNode(x - 1, y - 1);
			astarsearch->AddSuccessor(NewNode);
		}
		
		Node* pDownRight = pMap->getNode(x + 1, y + 1);
		if (pDownRight && (pDownRight->getTileCollisionId() == 0) 
			&& (pDownRight->getTileClearanceId() >= nAgentSize)
			&& !((parent_x == x + 1) && (parent_y == y + 1))
			&& pDown && (pDown->getTileCollisionId() == 0)
			&& pRight && (pRight->getTileCollisionId() == 0)
			)
		{
			NewNode = MapSearchNode(x + 1, y + 1);
			astarsearch->AddSuccessor(NewNode);
		}

		Node* pDownLeft = pMap->getNode(x - 1, y + 1);
		if (pDownLeft && (pMap->getNode(x - 1, y + 1)->getTileCollisionId() == 0) 
			&& (pDownLeft->getTileClearanceId() >= nAgentSize)
			&& !((parent_x == x - 1) && (parent_y == y + 1))
			&& pDown && (pDown->getTileCollisionId() == 0)
			&& pLeft && (pLeft->getTileCollisionId() == 0)
			)
		{
			NewNode = MapSearchNode(x - 1, y + 1);
			astarsearch->AddSuccessor(NewNode);
		}

		Node* pUpRight = pMap->getNode(x + 1, y - 1);
		if (pUpRight && (pMap->getNode(x + 1, y - 1)->getTileCollisionId() == 0)
			&& (pUpRight->getTileClearanceId() >= nAgentSize)
			&& !((parent_x == x + 1) && (parent_y == y - 1))
			&& pUp && (pUp->getTileCollisionId() == 0)
			&& pRight && (pRight->getTileCollisionId() == 0)
			)
		{
			NewNode = MapSearchNode(x + 1, y - 1);
			astarsearch->AddSuccessor(NewNode);
		}
		
		return true;
	}

	// given this node, what does it cost to move to successor. 
	float MapSearchNode::GetCost(MapSearchNode &successor)
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