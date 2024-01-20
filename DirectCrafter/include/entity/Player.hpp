#ifndef PLAYER_HPP
#define PLAYER_HPP

#define NOMINMAX

#include <algorithm>
#include "core/InputManager.hpp"
#include "render/Camera.hpp"

class Player
{

public:
	
	void Initialize(XMFLOAT3 position, XMFLOAT3 rotation)
	{
		InputManager::SetCursorLockMode(true);

		transform.position = position;
		transform.rotation = rotation;

		camera = Camera::Register(45.0f, 0.01f, 100.0f);
		camera.Update(transform.position);
	}

	void Update()
	{
		if(InputManager::GetKey(VirtualKey::Escape, KeyState::PRESSED))
			InputManager::SetCursorLockMode(false);

		UpdateMouseLook();

		transform.Update();
		camera.Update(transform.position);
	}

	Transform transform = Transform_Default;
	Camera camera = {};

private:

	void UpdateMouseLook()
	{
		static Point lastMousePosition = InputManager::GetMousePosition();
		Point currentMousePosition = InputManager::GetMousePosition();

		float deltaX = currentMousePosition.X - lastMousePosition.X;
		float deltaY = currentMousePosition.Y - lastMousePosition.Y;
		lastMousePosition = currentMousePosition;

		const float sensitivity = 0.0005f;

		camera.pitch = std::max(std::min(camera.pitch - deltaY * sensitivity, XM_PI / 2), -XM_PI / 2);
		camera.yaw += deltaX * sensitivity;
	}
};

#endif // !PLAYER_HPP