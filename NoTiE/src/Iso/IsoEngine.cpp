#include "IsoEngine.h"

IsoEngine::IsoEngine()
{
	ActiveMap = std::make_unique<Map>(Vector2Int(20, 20), Map::MapType::Diamond);
	Picker = std::make_unique<TilePicker>(*this);
}

IsoEngine::~IsoEngine()
{
	
}

Vector2Int IsoEngine::WorldToMapCoords(float x, float y)
{
	return Vector2Int
	{
		static_cast<int>(((x / (TILE_SIZE.x / 2) + y / (TILE_SIZE.y / 2)) / 2)),
		static_cast<int>(((y / (TILE_SIZE.y / 2) - (x / (TILE_SIZE.x / 2))) / 2))
	};
}

Vector2Int IsoEngine::WorldToMapCoords(Vector2Float worldCoords)
{
	return WorldToMapCoords(worldCoords.x, worldCoords.y);
}

Vector2Float IsoEngine::MapToWorldCoords(int x, int y)
{
	return Vector2Float
	{
		(x - y) * (TILE_SIZE.x / 2.0f),
		(x + y) * (TILE_SIZE.y / 2.0f)
	};
}

Vector2Float IsoEngine::MapToWorldCoords(Vector2Int mapCoords)
{
	return MapToWorldCoords(mapCoords.x, mapCoords.y);
}

void IsoEngine::Update()
{
	Picker->Update();
}

void IsoEngine::Draw(RenderSystem& renderSystem)
{
	ActiveMap->Draw(renderSystem);
	Picker->Draw(renderSystem);
}






