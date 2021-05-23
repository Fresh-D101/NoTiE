#include "Sprite.h"
#include <SFML/Graphics.hpp>

#include "../Resources/TextureManager.h"

Sprite::Sprite()
{
	NativeSprite = std::make_unique<sf::Sprite>();
}

Sprite::Sprite(std::string textureName)
{
	NativeSprite = std::make_unique<sf::Sprite>();
	SetTexture(textureName);
}


Sprite::~Sprite() = default;

Sprite::Sprite(const Sprite& other)
{
	const sf::Texture* texture = other.NativeSprite->getTexture();
	NativeSprite = std::make_unique<sf::Sprite>(*other.NativeSprite);

	if(texture != nullptr)
	{
		NativeSprite->setTexture(*texture);
	}
}

Sprite& Sprite::operator=(const Sprite& other)
{
	const sf::Texture* texture = other.NativeSprite->getTexture();
	NativeSprite = std::make_unique<sf::Sprite>(*other.NativeSprite);

	if(texture != nullptr)
	{
		NativeSprite->setTexture(*texture);
	}

	return *this;
}

sf::Sprite& Sprite::GetSprite() const
{
	return *NativeSprite.get();
}

void Sprite::SetTexture(const std::string& name)
{
	if (name == TextureName)
	{
		return;
	}

	TextureManager& textureManager = TextureManager::instance();
	sf::Texture* texture;
	if (!textureManager.IsLoaded(name))
	{
		texture = textureManager.Load(name);
	}
	else
	{
		texture = textureManager.GetResource(name);
	}

	if (TextureName != "")
	{
		textureManager.ReleaseResource(TextureName);
	}

	TextureName = name;
	NativeSprite->setTexture(*texture, true);
}

Vector2Int Sprite::GetSize() const
{
	sf::IntRect textureSize = NativeSprite->getTextureRect();
	return Vector2Int(textureSize.width, textureSize.height);
}

void Sprite::SetPivot(int x, int y)
{
	NativeSprite->setOrigin(x, y);
}

void Sprite::SetPosition(float x, float y)
{
	NativeSprite->setPosition(x, y);
}

void Sprite::Move(float x, float y)
{
	NativeSprite->move(x, y);
}

Vector2Float Sprite::GetPosition() const
{
	sf::Vector2f position = NativeSprite->getPosition();
	return Vector2Float(position.x, position.y);
}

void Sprite::Scale(const Vector2Float& factor)
{
	const sf::Vector2f scaleFactor{ factor.x, factor.y };
	NativeSprite->scale(scaleFactor);
}

const sf::Texture* Sprite::GetTexture() const
{
	return NativeSprite->getTexture();
}

