#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "render/Mesh.hpp"

namespace Renderer
{
	std::unordered_map<std::string, Mesh> registeredMeshes;

	void Register(Mesh mesh)
	{
		registeredMeshes.insert({ mesh.name, mesh });
	}

	void Render(Camera camera)
	{
		for (auto& [key, mesh] : registeredMeshes)
			mesh.Render(camera);
	}
};

#endif // !RENDERER_HPP