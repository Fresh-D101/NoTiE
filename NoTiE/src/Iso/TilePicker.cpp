#include "TilePicker.h"


#include "IsoEngine.h"
#include "../Core/Window.h"
#include "../Graphics/RenderSystem.h"

TilePicker::TilePicker(IsoEngine& isoEngine) : Map(*isoEngine.GetMap())
{
	isActive = true;
	isInBounds = false;
	
	SelectorSprite = Sprite();
	SelectorSprite.SetTexture("selected");
	SelectorSprite.Scale({ 0.25f, 0.25f });
}

void TilePicker::Update()
{
	if (!isActive) { return; }

	const Window& window = *RenderSystem::GetActiveWindow();
	const Vector2Int mousePos = Input::GetMousePositionInWindow(window);
	const Vector2Float worldCoords = window.ScreenToWorld(mousePos);
	
	selectedTile = IsoEngine::WorldToMapCoords(worldCoords.x, worldCoords.y);
	
	const Vector2Int mapSize = Map.GetSize();
	if ((selectedTile.x >= 0 && selectedTile.y >= 0) && selectedTile.x < mapSize.x && selectedTile.y < mapSize.y)
	{
		isInBounds = true;
		Vector2Float selectedPos = IsoEngine::MapToWorldCoords(selectedTile.x, selectedTile.y);
		selectedPos.x -= IsoEngine::TILE_SIZE.x / 2;
		SelectorSprite.SetPosition(selectedPos.x, selectedPos.y);
	}
	else
	{
		isInBounds = false;
	}
}

void TilePicker::Draw(RenderSystem& renderSystem)
{
	if(isActive && isInBounds)
	{
		renderSystem.GetWindow().DrawSprite(SelectorSprite);
	}
}


void TilePicker::SetActive(bool active)
{
	isActive = active;
}


