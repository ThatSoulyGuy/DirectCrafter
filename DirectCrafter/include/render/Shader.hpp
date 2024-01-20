#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <fstream>
#include <vector>
#include <d3dcompiler.h>
#include "core/GraphicsManager.hpp"

void PrintPath(const std::wstring& path)
{
    std::wstring fullPath = std::filesystem::current_path().wstring() + L"/Assets/" + Settings::domain + L"/" + path + L"Vertex.hlsl";
    std::wcout << fullPath << std::endl;
}

struct Shader
{
    ComPtr<ID3DBlob> vertexShaderBlob;
    ComPtr<ID3DBlob> pixelShaderBlob;
    ComPtr<ID3DBlob> errorMessage;
    ComPtr<ID3D11VertexShader> vertexShader;
    ComPtr<ID3D11PixelShader> pixelShader;

    std::string name;
    std::wstring vertexPath;
    std::wstring pixelPath;

    void Generate()
    {
        if (FAILED(D3DCompileFromFile(vertexPath.c_str(), nullptr, nullptr, "main", "vs_5_0", 0, 0, &vertexShaderBlob, &errorMessage)))
        {
            if (errorMessage)
                OutputDebugStringA((std::string("Shader Error: ") + std::string(reinterpret_cast<const char*>(errorMessage->GetBufferPointer()))).c_str());
            else
                OutputDebugStringA("Shader compilation failed, but no error message was provided.");

            return;
        }

        if (FAILED(D3DCompileFromFile(pixelPath.c_str(), nullptr, nullptr, "main", "ps_5_0", 0, 0, &pixelShaderBlob, &errorMessage)))
        {
            if (errorMessage)
                OutputDebugStringA((std::string("Shader Error: ") + std::string(reinterpret_cast<const char*>(errorMessage->GetBufferPointer()))).c_str());
            else
                OutputDebugStringA("Shader compilation failed, but no error message was provided.");

            return;
        }

        if (FAILED(GraphicsManager::device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader)))
        {
            if (errorMessage)
                OutputDebugStringA((std::string("Shader Error: ") + std::string(reinterpret_cast<const char*>(errorMessage->GetBufferPointer()))).c_str());
            else
                OutputDebugStringA("Failed to create the Vertex Shader Object.");

            return;
        }

        if (FAILED(GraphicsManager::device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader)))
        {
            if (errorMessage)
                OutputDebugStringA((std::string("Shader Error: ") + std::string(reinterpret_cast<const char*>(errorMessage->GetBufferPointer()))).c_str());
            else
                OutputDebugStringA("Failed to create the Pixel Shader Object.");

            return;
        }
    }

    void Activate() const
    {
        GraphicsManager::context->VSSetShader(vertexShader.Get(), nullptr, 0);
        GraphicsManager::context->PSSetShader(pixelShader.Get(), nullptr, 0);
    }

    static Shader Register(const std::wstring& path, const std::string& name)
    {
		Shader shader;

		shader.name = name;
		shader.vertexPath = std::filesystem::current_path().wstring() + L"/" + L"Assets/" + Settings::domain + L"/" + path + L"Vertex.hlsl";
        shader.pixelPath = std::filesystem::current_path().wstring() + L"/" + L"Assets/" + Settings::domain + L"/" + path + L"Pixel.hlsl";

		return shader;
    }
};

#endif // !SHADER_HPP