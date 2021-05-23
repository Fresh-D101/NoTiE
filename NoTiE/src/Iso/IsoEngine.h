#pragma once
#include <memory>

#include "Map.h"
#include "TilePicker.h"
#include "../Core/Vector2.h"

class IsoEngine
{
public:
	IsoEngine();
	~IsoEngine();

	//Conversion Functions
	static Vector2Int WorldToMapCoords(float x, float y);
	static Vector2Int WorldToMapCoords(Vector2Float worldCoords) ;
	static Vector2Float MapToWorldCoords(int x, int y);
	static Vector2Float MapToWorldCoords(Vector2Int mapCoords);

	void Update();
	void Draw(RenderSystem& renderSystem);

	//Access other Objects
	Map* GetMap() const { return ActiveMap.get(); };
	TilePicker* GetTilePicker() const { return Picker.get(); };
	
	static inline Vector2Int TILE_SIZE{ 64, 32 };
private:
	std::unique_ptr<TilePicker> Picker;
	std::unique_ptr<Map> ActiveMap;
};
