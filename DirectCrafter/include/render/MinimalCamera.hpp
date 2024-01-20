#ifndef MINIMAL_CAMERA_HPP
#define MINIMAL_CAMERA_HPP

#include <DirectXMath.h>
using namespace DirectX;

class MinimalCamera
{
public:

    void SetPosition(const XMFLOAT3& position) 
    {
        this->position = position;
    }

    void Update() 
    {
        const float rotationSpeed = 0.5f;
        yaw += rotationSpeed * InputManager::deltaTime;

        UpdateViewMatrix();
    }

    XMMATRIX GetViewMatrix() const 
    {
        return XMLoadFloat4x4(&viewMatrix);
    }

    XMMATRIX GetProjectionMatrix() const 
    {
        return XMLoadFloat4x4(&projectionMatrix);
    }

    static MinimalCamera Register(float fov, float nearPlane, float farPlane)
    {
        MinimalCamera out = {};

        out.fov = fov;
        out.nearPlane = nearPlane;
        out.farPlane = farPlane;

		out.UpdateViewMatrix();
		out.UpdateProjectionMatrix();

		return out;
	}

private:

    void UpdateViewMatrix() 
    {
        XMVECTOR positionVec = XMLoadFloat3(&this->position);

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

    XMFLOAT3 position = { 0.0f, 0.0f, -5.0f }; 
    float yaw = 0.0f;
    float pitch = 0.0f;

    float fov;
    float nearPlane;
    float farPlane;

    XMFLOAT4X4 viewMatrix;
    XMFLOAT4X4 projectionMatrix;
};

#endif // !MINIMAL_CAMERA_HPP