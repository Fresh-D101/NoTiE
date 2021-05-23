#include "Map.h"

#include <fstream>

#include "IsoEngine.h"
#include "../Core/Log.h"
#include "../Graphics/RenderSystem.h"

Map::Map(Vector2Int size, MapType type)
{
	auto standardTile = std::make_shared<TileInfo>();
	standardTile->Id = 0;
	standardTile->Name = "Standard Tile";
	standardTile->Texture = "ground";
	standardTile->Sprite.SetTexture(standardTile->Texture);
	TileInfos.emplace(standardTile->Id, standardTile);

	Size = size;
	
	Tiles.resize(Size.x * Size.y);

	for (int y = 0; y < Size.y; y++)
	{
		for (int x = 0; x < Size.x; x++)
		{
			Tile& tile = Tiles.at(y * Size.x + x);
			tile.Properties = standardTile;
			tile.Coord = Vector2Int(x, y);
		}
	}

	Type = type;
}

Map::~Map()
{
	
}

void Map::Draw(RenderSystem& system)
{
	Window& window = system.GetWindow();
	
	for (int y = 0; y < Size.y; y++)
	{
		for (int x = 0; x < Size.x; x++)
		{
			auto worldPos = IsoEngine::MapToWorldCoords(x, y);
			worldPos.x -= IsoEngine::TILE_SIZE.x / 2;
			
			Tile& tile = Tiles.at(y * Size.x + x);
			tile.Properties->Sprite.SetPosition(worldPos.x, worldPos.y);
			window.DrawSprite(tile.Properties->Sprite);
		}
	}
}

void Map::AddTileInfo(std::shared_ptr<TileInfo> info)
{
	TileInfos.try_emplace(info->Id, info);
}

void Map::SetTileAt(Vector2Int tilePos, TileID newTileId)
{
	Tile& tile = Tiles.at(tilePos.y * Size.x + tilePos.x);
	auto info = TileInfos.at(newTileId);
	if(info)
	{
		tile.Properties = info;
	}
}

void Map::Serialize(std::ostream& fileStream)
{
	//Write a signature for check
	char signature[] = "NoTiE Map File";
	fileStream.write(signature, sizeof(signature));

	//The Map Type
	fileStream.write(reinterpret_cast<char*>(&Type), sizeof MapType);
	
	//The Map size
	fileStream.write(reinterpret_cast<char*>(&Size), sizeof Vector2Int);

	//The Tiles
	for (int y = 0; y < Size.y; y++)
	{
		for (int x = 0; x < Size.x; x++)
		{
			TileID tileInfoId = Tiles.at(y * Size.x + x).Properties->Id;
			fileStream.write(reinterpret_cast<char*>(&tileInfoId), sizeof TileID);
		}
	}
}

void Map::Load(std::istream& fileStream)
{
	char signature[15];
	fileStream.read(signature, 15);

	if (strcmp(signature, "NoTiE Map File") != 0)
	{
		NTE_CORE_ERROR("Cannot open file, unknown map format!");
		return;
	}

	//Clear only after confirmation of correct file
	Tiles.clear();
	
	//Read the Map Type
	fileStream.read(reinterpret_cast<char*>(&Type), sizeof MapType);
	
	//Read the Size
	fileStream.read(reinterpret_cast<char*>(&Size), sizeof Vector2Int);
	
	Tiles.resize(Size.x * Size.y);
	
	for (int y = 0; y < Size.y; y++)
	{
		for (int x = 0; x < Size.x; x++)
		{
			Tile& tile = Tiles.at(y * Size.x + x);
			TileID tileInfoId;
			fileStream.read(reinterpret_cast<char*>(&tileInfoId), sizeof TileID);
			tile.Properties = TileInfos.at(tileInfoId);
			tile.Coord = Vector2Int(x, y);
		}
	}
}