#include "stdafx.h"

#include "Data\TileSet.h"
#include <string>

namespace crea
{
	TileSet::TileSet()
	{
	}

	TileSet::~TileSet()
	{
		VectorTerrain::iterator it = m_Terrains.begin();
		while (it != m_Terrains.end()) {
			delete (*it);
			it = m_Terrains.erase(it);
		}

		MapTileInfo::iterator itTile = m_TileInfos.begin();
		while (itTile != m_TileInfos.end()) {
			delete (*itTile).second;
			itTile = m_TileInfos.erase(itTile);
		}
	}

	float TileSet::getFriction(unsigned short _nTileId, unsigned short _nQuad)
	{
		TileInfo* pTileInfo = m_TileInfos[to_string(_nTileId)];
		unsigned short nTerrain = pTileInfo->m_nTerrain[_nQuad];
		return m_Terrains[nTerrain]->m_fFriction;
	}
}