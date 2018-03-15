/***************************************************/
/* Nom:	Map.h
/* Description: Map
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __Map_H_
#define __Map_H_

#include "TileSet.h"

namespace crea
{
	class Node;
	class Cluster;

	class CREAENGINE_API Map
	{
		crea::GameManager*	m_pGM;

		// Name
		string m_szName;

		// Dimensions
		short m_nWidth;
		short m_nHeight;
		short m_nTileWidth;
		short m_nTileHeight;

		// Tilesets
		vector<TileSet*> m_TileSet;
		TileSet* m_pTerrainTileSet;

		// Nodes
		Node* **m_Grid;	// Allocation dynamique
		bool m_bIsGrid8;

		// Clusters
		bool m_bUseHPA;
		short m_nClusterWidth;
		short m_nClusterHeight;
		short m_nNbClustersX;
		short m_nNbClustersY;
		Cluster* **m_Clusters;

		// Draw modes
		bool m_bDisplayCollision;

		// Tile index limits
		int m_iMin;
		int m_iMax;
		int m_jMin;
		int m_jMax;

		void updateTileIndexLimits();

	public:

		Map();

		~Map();

		bool loadFromFileJSON(string& _filename);

		inline void setName(string& _szName) { m_szName = _szName; }

		void setSize(short _nWidth, short _nHeight);

		void getSize(short& _nWidth, short& _nHeight);

		void getTileSize(short& _nWidth, short& _nHeight);

		void setClusterSize(short _nClusterWidth, short _nClusterHeight);

		void getClusterSize(short& _nClusterWidth, short& _nClusterHeight);

		Cluster* getCluster(short _i, short _j) { return m_Clusters[_i][_j]; }

		void getClusterIndexLimits(int& _iMin, int& _iMax, int& _jMin, int& _jMax) { _iMin = 0; _iMax = m_nNbClustersX; _jMin = 0; _jMax = m_nNbClustersY; }

		void limitClusterIndex(short& _i, short& _j) { _i = _i < 0 ? 0 : _i; _i = _i > m_nNbClustersX ? m_nNbClustersX : _i; _j = _j < 0 ? 0 : _j; _j = _j > m_nNbClustersY ? m_nNbClustersY : _j; }

		Node* getNodeInCluster(Cluster* _pCluster, short _i, short _j);

		void setNode(short _i, short _j, Node* _pNode) { m_Grid[_i][_j] = _pNode; }

		Node* getNode(short _i, short _j) { /*limitTileIndex(_i, _j);*/  return (isInLimitTileIndex(_i, _j) ? m_Grid[_i][_j] : nullptr); }

		TileSet* getTileSet(short _gid);

		Node* getNodeAtPosition(Vector2f _v);

		unsigned short getQuadAtPosition(Vector2f& _v);
			
		Vector2f getNodePositionFromPixels(Vector2f _v);

		Vector2f getPixelsFromNodePosition(Vector2f _v);

		float getFrictionAtPosition(Vector2f _v);

		void getTileIndexLimits(int& _iMin, int& _iMax, int& _jMin, int& _jMax) { _iMin = m_iMin; _iMax = m_iMax; _jMin = m_jMin; _jMax = m_jMax; }
		
		void limitTileIndex(short& _i, short& _j) { _i = _i < m_iMin ? m_iMin : _i; _i = _i > m_iMax ? m_iMax : _i; _j = _j < m_jMin ? m_jMin : _j; _j = _j > m_jMax ? m_jMax : _j;}

		bool isInLimitTileIndex(short& _i, short& _j) { return (_i >= m_iMin && _i <= m_iMax && _j >= m_jMin && _j <= m_jMax); }

		bool init();

		bool update();

		bool draw();

		void clear();
	};

} // namespace crea

#endif
