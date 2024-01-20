#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <DirectXMath.h>
using namespace DirectX;

#define Transform_Default Transform::Register({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f })

struct Transform
{
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;

	XMFLOAT3 forward;
	XMFLOAT3 right;
	XMFLOAT3 up;

	Transform()
	{
		position = { 0.0f, 0.0f, 0.0f };
		rotation = { 0.0f, 0.0f, 0.0f };
		scale = { 1.0f, 1.0f, 1.0f };

		forward = { 0.0f, 0.0f, 1.0f };
		right = { 0.0f, 0.0f, 0.0f };
		up = { 0.0f, 1.0f, 0.0f };
	}

	void Update()
	{
		XMVECTOR rotation = XMLoadFloat3(&this->rotation);
		XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYawFromVector(rotation);

		XMVECTOR localForward = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationMatrix);
		XMVECTOR localRight = XMVector3TransformCoord(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rotationMatrix);
		XMVECTOR localUp = XMVector3TransformCoord(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotationMatrix);

		XMStoreFloat3(&forward, XMVector3Normalize(localForward));
		XMStoreFloat3(&right, XMVector3Normalize(localRight));
		XMStoreFloat3(&up, XMVector3Normalize(localUp));
	}

	XMMATRIX GetMatrix_0_1_2() const
	{
		XMVECTOR position = XMLoadFloat3(&this->position);
		XMVECTOR rotation = XMLoadFloat3(&this->rotation);
		XMVECTOR scale = XMLoadFloat3(&this->scale);

		XMMATRIX positionMatrix = XMMatrixTranslationFromVector(position);
		XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYawFromVector(rotation);
		XMMATRIX scaleMatrix = XMMatrixScalingFromVector(scale);
		
		return positionMatrix * rotationMatrix * scaleMatrix;
	}

	XMMATRIX GetMatrix_2_1_0() const
	{
		XMVECTOR position = XMLoadFloat3(&this->position);
		XMVECTOR rotation = XMLoadFloat3(&this->rotation);
		XMVECTOR scale = XMLoadFloat3(&this->scale);

		XMMATRIX positionMatrix = XMMatrixTranslationFromVector(position);
		XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYawFromVector(rotation);
		XMMATRIX scaleMatrix = XMMatrixScalingFromVector(scale);

		return scaleMatrix * rotationMatrix * positionMatrix;
	}

	static Transform Register(const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale)
	{
		Transform out = {};

		out.position = position;
		out.rotation = rotation;
		out.scale = scale;
		out.Update();

		return out;
	}
};

#endif // !TRANSFORM_HPP