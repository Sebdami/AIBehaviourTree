#include "stdafx.h"

#include "Data\Cluster.h"

namespace crea
{
	Cluster::Cluster()
	{
		m_pTopCluster = nullptr;
		m_pBottomCluster = nullptr;
		m_pLeftCluster = nullptr;
		m_pRightCluster = nullptr;

		m_pEntrances = new vector<Entrance*>();
		m_pTransitions = new vector<Transition*>();
		m_pEdges = new vector<Edge*>();
	}

	Cluster::~Cluster()
	{
		for (size_t e = 0; e < m_pEntrances->size(); e++)
		{
			Entrance* pEntrance = (*m_pEntrances)[e];
			delete pEntrance;
		}
		delete m_pEntrances;		
		
		for (size_t t = 0; t < m_pTransitions->size(); t++)
		{
			Transition* pTransition = (*m_pTransitions)[t];
			delete pTransition;
		}
		delete m_pEntrances;

		for (size_t e = 0; e < m_pEdges->size(); e++)
		{
			Edge* pEdge = (*m_pEdges)[e];
			delete pEdge;
		}
		delete m_pEdges;
	}

	bool Cluster::update()
	{
		return true;
	}

	bool Cluster::draw()
	{
		return true;
	}

	void Cluster::clear()
	{
	}

	void Cluster::findEntrances()
	{
		Map* pMap = MapSearchManager::getSingleton()->getCurrentMap();
		short nWidth, nHeigth;

		// Top line
		Entrance* pEntrance = nullptr;
		Entrance* pEntranceSymm = nullptr;
		if (m_pTopCluster)
		{
			short j = 0;
			// E appartient à la bordure de c1 et c2
			for (int i = 0; i < m_nClusterWidth; i++)
			{
				m_pTopCluster->getSize(nWidth, nHeigth);

				// E appartient à c1, symm(E) appartient à c2 
				Node* pNode1 = pMap->getNodeInCluster(this, i, j);
				Node* pNode2 = pMap->getNodeInCluster(m_pTopCluster, i, nHeigth - 1);

				// E n'a pas d'obstacles
				if (pNode1->getTileCollisionId() == 0 && pNode2->getTileCollisionId() == 0)
				{
					// E s'étend des 2 côtés
					if (!pEntrance)
					{
						// Start entrance
						pEntrance = new Entrance;
						pEntrance->m_pStart = pNode1;
						pEntrance->m_pEnd = pNode1;
						m_pEntrances->push_back(pEntrance);

						// Set symmetric
						pEntranceSymm = new Entrance;
						pEntranceSymm->m_pStart = pNode2;
						pEntranceSymm->m_pEnd = pNode2;
						m_pTopCluster->m_pEntrances->push_back(pEntranceSymm);
						pEntrance->m_pSymm = pEntranceSymm;
						pEntranceSymm->m_pSymm = pEntrance;
					}
					else
					{
						pEntrance->m_pEnd = pNode1;
						pEntranceSymm->m_pEnd = pNode2;
					}
				}
				else
				{
					pEntrance = nullptr;
					pEntranceSymm = nullptr;
				}
			}
		}

		// Left line
		pEntrance = nullptr;
		pEntranceSymm = nullptr;
		if (m_pLeftCluster)
		{
			short i = 0;
			// E appartient à la bordure de c1 et c2
			for (int j = 0; j < m_nClusterHeight; j++)
			{
				m_pLeftCluster->getSize(nWidth, nHeigth);

				// E appartient à c1, symm(E) appartient à c2 
				Node* pNode1 = pMap->getNodeInCluster(this, i, j);
				Node* pNode2 = pMap->getNodeInCluster(m_pLeftCluster, nWidth - 1, j);

				// E n'a pas d'obstacles
				if (pNode1->getTileCollisionId() == 0 && pNode2->getTileCollisionId() == 0)
				{
					// E s'étend des 2 côtés
					if (!pEntrance)
					{
						// Start entrance
						pEntrance = new Entrance;
						pEntrance->m_pStart = pNode1; 
						pEntrance->m_pEnd = pNode1;
						m_pEntrances->push_back(pEntrance);

						// Set symmetric
						pEntranceSymm = new Entrance;
						pEntranceSymm->m_pStart = pNode2;
						pEntranceSymm->m_pEnd = pNode2;
						m_pLeftCluster->m_pEntrances->push_back(pEntranceSymm);
						pEntrance->m_pSymm = pEntranceSymm;
						pEntranceSymm->m_pSymm = pEntrance;
					}
					else
					{
						pEntrance->m_pEnd = pNode1;
						pEntranceSymm->m_pEnd = pNode2;
					}
				}
				else
				{
					pEntrance = nullptr;
					pEntranceSymm = nullptr;
				}
			}
		}

	}

	void Cluster::findTransitions()
	{
		Map* pMap = MapSearchManager::getSingleton()->getCurrentMap();

		for (size_t e = 0; e < m_pEntrances->size(); e++)
		{
			Entrance* pEntrance = (*m_pEntrances)[e];
			float distance = (float) pEntrance->m_pStart->getDistance(pEntrance->m_pEnd);
			if (distance < 5.f) // < 6 tiles means < 5 in distance
			{
				Transition* t = new Transition;
				t->m_pStart = pMap->getNode(
					(short)((pEntrance->m_pEnd->getX() + pEntrance->m_pStart->getX()) * 0.5f),
					(short)((pEntrance->m_pEnd->getY() + pEntrance->m_pStart->getY()) * 0.5f));
				t->m_pEnd = pMap->getNode(
					(short)((pEntrance->m_pSymm->m_pEnd->getX() + pEntrance->m_pSymm->m_pStart->getX()) * 0.5f),
					(short)((pEntrance->m_pSymm->m_pEnd->getY() + pEntrance->m_pSymm->m_pStart->getY()) * 0.5f));
				t->m_pEdge = new Edge;
				t->m_pEdge->m_fDistance = 1.0f;
				m_pTransitions->push_back(t);
			}
			else
			{
				Transition* t1 = new Transition;
				t1->m_pStart = pEntrance->m_pStart;
				t1->m_pEnd = pEntrance->m_pSymm->m_pStart;
				t1->m_pEdge = new Edge;
				t1->m_pEdge->m_fDistance = 1.0f;
				m_pTransitions->push_back(t1);

				Transition* t2 = new Transition;
				t2->m_pStart = pEntrance->m_pEnd;
				t2->m_pEnd = pEntrance->m_pSymm->m_pEnd;
				t2->m_pEdge = new Edge;
				t2->m_pEdge->m_fDistance = 1.0f;
				m_pTransitions->push_back(t2);
			}
		}
	}

	void Cluster::findEdges()
	{
		GameManager* pGM = GameManager::getSingleton();
		ClusterSearchManager* pClusterSearchManager = ClusterSearchManager::getSingleton();
		Map* pMap = MapSearchManager::getSingleton()->getCurrentMap();
		pClusterSearchManager->setCurrentMap(pMap);

		ClusterSearch* pClusterSearch = pGM->getClusterSearch("Cluster::findEdges");

		for (unsigned int t1 = 0; t1 < m_pTransitions->size(); t1++)
		{
			Transition* pTransition1 = (*m_pTransitions)[t1];

			for (unsigned int t2 = t1+1; t2 < m_pTransitions->size(); t2++)
			{
				Transition* pTransition2 = (*m_pTransitions)[t2];
				// Edge from transition start to others transition start in cluster
				Edge* pIntraEdge = new Edge;
				pIntraEdge->m_pStart = pTransition1->m_pStart;
				pIntraEdge->m_pEnd = pTransition2->m_pStart;

				// CB: to do: all searches find a solution as we are still using map, not cluster!

				// Pathfinding in cluster!
				pClusterSearchManager->setCurrentCluster(this);

				ClusterSearch::SearchState s;
				pClusterSearch->setStartAndGoal(
					Vector2f(pIntraEdge->m_pStart->getX(), pIntraEdge->m_pStart->getY()),
					Vector2f(pIntraEdge->m_pEnd->getX(), pIntraEdge->m_pEnd->getY()), 
					false // not from pixels as we already have indexes!
				);
				do
				{
					s = pClusterSearch->update();
				} while (s == MapSearch::SEARCH_STATE_SEARCHING);

				if (s == MapSearch::SEARCH_STATE_SUCCEEDED)
				{
					pIntraEdge->m_fDistance = pClusterSearch->getSolutionLength(); // result of pathfinding!
					m_pEdges->push_back(pIntraEdge);
				}
			}
		}
	}

	bool Cluster::isInCluster(short _i, short _j)
	{
		short dx = m_pLeftCluster ? _i - m_nX*m_pLeftCluster->m_nClusterWidth : _i - m_nX*m_nClusterWidth;
		short dy = m_pTopCluster ? _j - m_nY*m_pTopCluster->m_nClusterHeight : _j - m_nY*m_nClusterHeight;
		return (dx >= 0 && dx < m_nClusterWidth && dy >= 0 && dy < m_nClusterHeight);
	}


} // namespace crea
