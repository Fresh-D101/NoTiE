#pragma once
#include <memory>
#include <string>

#include "../Core/Vector2.h"

namespace sf
{
	class Texture;
	class Sprite;
}

class Sprite
{
public:
	Sprite();
	Sprite(std::string textureName);
	~Sprite();
	
	Sprite(const Sprite& other);
	Sprite& operator= (const Sprite& other);
	
public:
	void SetTexture(const std::string & name);
	sf::Sprite& GetSprite() const;
	Vector2Int GetSize() const;

	//Set the pivot of the sprite in local space
	void SetPivot(int x, int y);

	void SetPosition(float x, float y);
	Vector2Float GetPosition() const;
	void Move(float x, float y);

	void Scale(const Vector2Float& factor);

	const sf::Texture* GetTexture() const;

private:
	std::unique_ptr<sf::Sprite> NativeSprite;
	std::string TextureName;
};
