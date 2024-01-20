#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "core/Settings.hpp"
#include "math/Transform.hpp"

class Camera
{

public:
	
	void Update(const XMFLOAT3& _position)
	{
		transform.position = _position;

		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}
	
	XMMATRIX GetViewMatrix() const
	{
		return XMLoadFloat4x4(&viewMatrix);
	}

	XMMATRIX GetProjectionMatrix() const
	{
		return XMLoadFloat4x4(&projectionMatrix);
	}

	static Camera Register(float fov, float nearPlane, float farPlane)
	{
		Camera out = {};

		out.fov = fov;
		out.nearPlane = nearPlane;
		out.farPlane = farPlane;

		return out;
	}
	
	float fov = 0;
	float nearPlane = 0;
	float farPlane = 0;
	float pitch = 0;
	float yaw = 0;

	Transform transform = Transform_Default;

private:

	void UpdateViewMatrix()
	{
		XMVECTOR positionVec = XMLoadFloat3(&this->transform.position);

		XMVECTOR forward = XMVectorSet(
			sinf(yaw) * cosf(pitch),
			sinf(pitch),
			cosf(yaw) * cosf(pitch),
			0.0f
		);
		forward = XMVector3Normalize(forward);

		XMVECTOR target = positionVec + forward;

		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		XMMATRIX view = XMMatrixLookAtLH(positionVec, target, up);
		XMStoreFloat4x4(&this->viewMatrix, XMMatrixTranspose(view));
	}

	void UpdateProjectionMatrix()
	{
		XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), (float)Settings::window.Bounds().Width / (float)Settings::window.Bounds().Height, nearPlane, farPlane);
		XMStoreFloat4x4(&this->projectionMatrix, XMMatrixTranspose(projectionMatrix));
	}

	DirectX::XMFLOAT4X4 viewMatrix = {};
	DirectX::XMFLOAT4X4 projectionMatrix = {};

};

#endif // !CAMERA_HPP