#pragma once
#include <unordered_map>
#include <vector>

#include "Tile.h"
#include "../Core/Vector2.h"

class RenderSystem;

class Map
{
public:
	enum class MapType
	{
		Slide,
		Staggered,
		Diamond
	};

	
	Map(Vector2Int size, MapType type);
	~Map();
	
	void Draw(RenderSystem& system);

	//Adding new TileInfos
	void AddTileInfo(std::shared_ptr<TileInfo> info);
	void SetTileAt(Vector2Int tilePos, TileID newTileId);

	Vector2Int GetSize() const { return Size; }

	void Serialize(std::ostream& fileStream);
	void Load(std::istream& fileStream);
private:
	std::vector<Tile> Tiles;
	std::unordered_map <TileID, std::shared_ptr<TileInfo>> TileInfos;
	
	Vector2Int Size;
	MapType Type;
};
