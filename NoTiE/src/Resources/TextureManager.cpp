#include "TextureManager.h"
#include <SFML/Graphics.hpp>
#include <iostream>

TextureManager* TextureManager::Instance = nullptr;

TextureManager::TextureManager() = default;
TextureManager::~TextureManager()
{
	delete Instance;
};

TextureManager& TextureManager::instance()
{
	if (Instance == nullptr) Instance = new TextureManager();
	return *Instance;
}

sf::Texture* TextureManager::Load(const std::string& name)
{
	auto path = Paths.find(name);
	if (path == Paths.end())
	{
		return nullptr;
	}

	auto texture = std::make_unique<sf::Texture>();
	if (!texture->loadFromFile(path->second))
	{
		std::cerr << "! Failed to load texture: " << path->first << std::endl;
	}

	sf::Texture* loadedTexture = texture.get();
	Resources.emplace(name,  std::move(std::make_pair(std::move(texture), 1)));
	
	return loadedTexture;
}
