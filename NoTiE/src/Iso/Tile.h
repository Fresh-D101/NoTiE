#pragma once
#include "../Graphics/Sprite.h"

using TileID = unsigned int;

struct TileInfo
{
	Sprite Sprite;
	TileID Id;
	std::string Name;
	std::string Texture;
};

struct Tile
{
	std::shared_ptr<TileInfo> Properties;
	Vector2Int Coord;
};