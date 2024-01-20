#ifndef SHADER_MANAGER_HPP
#define SHADER_MANAGER_HPP

#include <unordered_map>
#include "render/Shader.hpp"

namespace ShaderManager
{
	std::unordered_map<std::string, Shader> registeredShaders;

	void Register(const Shader& shader)
	{
		registeredShaders.insert({shader.name, shader});
	}

	Shader Get(const std::string& name)
	{
		return registeredShaders[name];
	}
}

#endif // !SHADER_MANAGER_HPP