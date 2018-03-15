/***************************************************/
/* Nom:	Cluster.h
/* Description: Cluster
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __Cluster_H_
#define __Cluster_H_

#include <vector>

namespace crea
{
	class CREAENGINE_API Edge
	{
	public:
		Node* m_pStart;
		Node* m_pEnd;
		float m_fDistance;
	};

	class CREAENGINE_API Entrance
	{
	public:
		Node* m_pStart;
		Node* m_pEnd;
		Entrance* m_pSymm;
	};

	class CREAENGINE_API Transition
	{
	public:
		Node* m_pStart;
		Node* m_pEnd;
		Edge* m_pEdge;
	};

	class CREAENGINE_API Cluster
	{
		// Position
		short m_nX;
		short m_nY;

		// Size
		short m_nClusterWidth;
		short m_nClusterHeight;

		// Neighbors
		Cluster* m_pTopCluster;
		Cluster* m_pBottomCluster;
		Cluster* m_pLeftCluster;
		Cluster* m_pRightCluster;

		// Entrances
		vector<Entrance*>* m_pEntrances;

		// Transitions
		vector<Transition*>* m_pTransitions;

		// Edges
		vector<Edge*>* m_pEdges;

	public:
		Cluster();
		virtual ~Cluster();
		
		void setPosition(short _nX, short _nY) { m_nX = _nX; m_nY = _nY; }

		void getPosition(short& _nX, short& _nY) { _nX = m_nX; _nY = m_nY; }

		void setSize(short _nClusterWidth, short _nClusterHeight) { m_nClusterWidth = _nClusterWidth; m_nClusterHeight = _nClusterHeight; }

		void getSize(short& _nClusterWidth, short& _nClusterHeight) { _nClusterWidth = m_nClusterWidth; _nClusterHeight = m_nClusterHeight; }

		bool isInCluster(short _i, short _j);

		void addTopCluster(Cluster* _pCluster) { m_pTopCluster = _pCluster; }

		void addBottomCluster(Cluster* _pCluster) { m_pBottomCluster = _pCluster; }

		void addLeftCluster(Cluster* _pCluster) { m_pLeftCluster = _pCluster; }

		void addRightCluster(Cluster* _pCluster) { m_pRightCluster = _pCluster; }

		void findEntrances();

		void findTransitions();

		void findEdges();

		vector<Entrance*>* getEntrances() { return m_pEntrances; }
		
		vector<Transition*>* getTransitions() { return m_pTransitions; }

		vector<Edge*>* getEdges() { return m_pEdges; }

		bool update();

		bool draw();

		void clear();
	};

} // namespace crea

#endif
