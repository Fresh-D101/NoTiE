#pragma once
#include "Map.h"
#include "../Graphics/Sprite.h"


class IsoEngine;

class TilePicker
{
public:
	TilePicker(IsoEngine& parent);
	~TilePicker() = default;
	
	void Update();
	void Draw(RenderSystem& renderSystem);
	void SetActive(bool active);

	Vector2Int GetSelectedTile() const { return selectedTile; }
	bool IsInMapBounds() const { return isInBounds; }
private:
	bool isInBounds;
	bool isActive;
	Sprite SelectorSprite;
	Map& Map;
	Vector2Int selectedTile {-1, -1};
};
