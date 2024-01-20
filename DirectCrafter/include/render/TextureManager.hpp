#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include "render/Texture.hpp"

namespace TextureManager
{
	std::unordered_map<std::string, Texture> registeredTextures;

	void Register(Texture texture)
	{
		registeredTextures.insert({ texture.name, texture });
	}

	Texture Get(const std::string& name)
	{
		return registeredTextures[name];
	}
};

#endif // !TEXTURE_MANAGER_HPP