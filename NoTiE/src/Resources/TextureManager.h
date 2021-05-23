#pragma once

#include "ResourceManager.h"

namespace sf
{
	class Texture;
}

class TextureManager : public ResourceManager<TextureManager, sf::Texture>
{
public:
	static TextureManager& instance();

	sf::Texture* Load(const std::string& name);
	
private:
	TextureManager();
	~TextureManager();
	
	static TextureManager* Instance;
};
