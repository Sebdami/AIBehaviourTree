/***************************************************/
/* Nom:	Node.h
/* Description: Node
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __Node_H_
#define __Node_H_

#include <vector>

namespace crea
{

	class CREAENGINE_API Node
	{
		// TileId
		short m_nTileTerrainId;
		short m_nTileCollisionId;
		short m_nTileClearanceId;

		// Position
		short m_nX;
		short m_nY;

		// Neighbors
		vector<Node*> m_pChildren;
		
	public:
		Node();
		Node(short _nX, short _nY)								{ m_nX = _nX; m_nY = _nY; }
		virtual ~Node();
		
		inline void setX(short _nX) { m_nX = _nX; }

		inline short getX() { return  m_nX; }

		inline void setY(short _nY) { m_nY = _nY; }

		inline short getY() { return  m_nY; }

		inline void setTileTerrainId(short _nTileTerrainId)		{ m_nTileTerrainId = _nTileTerrainId; }

		inline short getTileTerrainId()							{ return  m_nTileTerrainId; }

		inline void setTileCollisionId(short _nTileCollisionId)	{ m_nTileCollisionId = _nTileCollisionId; }

		inline short getTileCollisionId()						{ return  m_nTileCollisionId; }

		inline void setTileClearanceId(short _nTileClearanceId) { m_nTileClearanceId = _nTileClearanceId; }

		inline short getTileClearanceId() { return  m_nTileClearanceId; }

		double getDistance(Node* _pNode) { short dx = m_nX - _pNode->getX(); short dy = m_nY - _pNode->getY(); return sqrt(dx*dx + dy*dy); }
		
		void addChild(Node* _pNode);
		
		bool update();

		bool draw();

		void clear();
	};

} // namespace crea

#endif
