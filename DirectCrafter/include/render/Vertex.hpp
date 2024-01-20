#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <DirectXMath.h>

struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;
	DirectX::XMFLOAT2 textureCoordinates;
	
	static Vertex Register(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& textureCoordinates)
	{
		return Register(position, { 1.0f, 1.0f, 1.0f }, textureCoordinates);
	}

	static Vertex Register(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& color, const DirectX::XMFLOAT2& textureCoordinates)
	{
		Vertex out = {};

		out.position = position;
		out.color = color;
		out.textureCoordinates = textureCoordinates;

		return out;
	}
};

#endif // !VERTEX_HPP