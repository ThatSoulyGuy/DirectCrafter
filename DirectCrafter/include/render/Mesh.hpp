#ifndef MESH_HPP
#define MESH_HPP

#include "core/GraphicsManager.hpp"
#include "math/Transform.hpp"
#include "render/MinimalCamera.hpp"
#include "render/ShaderManager.hpp"
#include "render/TextureManager.hpp"
#include "render/Vertex.hpp"

struct MatrixBufferType 
{
    DirectX::XMMATRIX model;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
};

struct Mesh
{
    void GenerateTriangle()
    {
        vertices =
        {
            Vertex::Register({ 0.0f,  0.5f, 0.0f}, {1.0f, 0.0f}),
            Vertex::Register({ 0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}),
            Vertex::Register({-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f})
        };

        indices =
        {
            0, 1, 2
        };

        RegisterTexture("test");
        Generate();
    }

    void GenerateSquare()
    {
		vertices =
        {
            Vertex::Register({-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f}),
            Vertex::Register({ 0.5f,  0.5f, 0.0f}, {1.0f, 0.0f}),
            Vertex::Register({-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}),
            Vertex::Register({ 0.5f, -0.5f, 0.0f}, {1.0f, 1.0f})
		};

		indices =
        {
			0, 1, 2,
			2, 1, 3
		};

        RegisterTexture("test");
		Generate();
	}

    void Generate()
    {
        shader.Generate();

        D3D11_BUFFER_DESC matrixBufferDesc = {};
        matrixBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
        matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        matrixBufferDesc.CPUAccessFlags = 0;
        matrixBufferDesc.MiscFlags = 0;
        matrixBufferDesc.StructureByteStride = 0;

        HRESULT result = GraphicsManager::device->CreateBuffer(&matrixBufferDesc, nullptr, &matrixBuffer);

        if (FAILED(result))
        {
            throw DXWIN_EXCEPTION(result);
            return;
        }

        for (auto& texture : textures)
            texture.second.Generate(D3D11_FILTER_MIN_MAG_MIP_POINT);

        D3D11_BUFFER_DESC vertexBufferDescription = {};
        vertexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDescription.ByteWidth = sizeof(Vertex) * vertices.size();
        vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDescription.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA vertexBufferData = {};
        vertexBufferData.pSysMem = vertices.data();

        if (FAILED(GraphicsManager::device->CreateBuffer(&vertexBufferDescription, &vertexBufferData, &vertexBuffer)))
        {
            OutputDebugStringA("Failed to create vertex buffer.");
            return;
        }

        D3D11_BUFFER_DESC indexBufferDesc = {};
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(unsigned int) * indices.size();
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA indexBufferData = {};
        indexBufferData.pSysMem = indices.data();

        if (FAILED(GraphicsManager::device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer)))
        {
            OutputDebugStringA("Failed to create index buffer.");
            return;
        }

        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXTURE_COORDINATES", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        if (FAILED(GraphicsManager::device->CreateInputLayout(layout, ARRAYSIZE(layout), shader.vertexShaderBlob->GetBufferPointer(), shader.vertexShaderBlob->GetBufferSize(), &inputLayout)))
        {
            OutputDebugStringA("Failed to create input layout.");
            return;
        }
    }

    void RegisterTexture(const std::string& name)
    {
		textures.insert({ name, TextureManager::Get(name) });
	}

    void Render(Camera camera)
    {
        shader.Activate();

        int count = 0;

        for (auto& [key, texture] : textures)
        {
            texture.Bind(count);

            ++count;
        }

        XMVECTOR positionVector = XMLoadFloat3(&transform.position);

        XMMATRIX translationMatrix = XMMatrixTranslationFromVector(positionVector);

        MatrixBufferType matrices = {};

        matrices.model = translationMatrix;
        matrices.view = camera.GetViewMatrix();
        matrices.projection = camera.GetProjectionMatrix();

        GraphicsManager::context->UpdateSubresource(matrixBuffer.Get(), 0, nullptr, &matrices, 0, 0);
        GraphicsManager::context->VSSetConstantBuffers(0, 1, matrixBuffer.GetAddressOf());

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        GraphicsManager::context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
        GraphicsManager::context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

        GraphicsManager::context->IASetInputLayout(inputLayout.Get());
        GraphicsManager::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        GraphicsManager::context->DrawIndexed(indices.size(), 0, 0);
    }

    static Mesh Register(const std::string& name, const std::vector<Vertex> vertices, const std::vector<unsigned int> indices, const std::string& shader)
    {
        Mesh out = {};

        out.name = name;
        out.vertices = vertices;
        out.indices = indices;
        out.shader = ShaderManager::Get(shader);

        return out;

    }

    std::string name;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::unordered_map<std::string, Texture> textures;
    Transform transform = Transform_Default;
    Shader shader = {};

private:

    ComPtr<ID3D11PixelShader> pixelShader;
    ComPtr<ID3D11InputLayout> inputLayout;
    ComPtr<ID3D11Buffer> vertexBuffer;
    ComPtr<ID3D11Buffer> indexBuffer;
    mutable ComPtr<ID3D11Buffer> matrixBuffer;
    mutable ComPtr<ID3D11SamplerState> samplerState;
};

#endif // !MESH_HPP