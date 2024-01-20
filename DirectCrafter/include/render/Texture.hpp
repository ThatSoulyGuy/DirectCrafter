#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <codecvt>
#include <locale>
#include <WICTextureLoader.h>
#include "core/GraphicsManager.hpp"

std::string WSTR2STR(const std::wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(wstr);
}

typedef D3D11_FILTER TextureFilter;

struct Texture
{

public:

	std::string name;
	std::string path;
	
	void Generate(TextureFilter filtering = D3D11_FILTER_MIN_MAG_MIP_LINEAR)
	{
		HRESULT result = DirectX::CreateWICTextureFromFile(GraphicsManager::device.Get(), std::wstring(path.begin(), path.end()).c_str(), nullptr, textureView.GetAddressOf());

		if (FAILED(result))
		{
			DXWIN_EXCEPTION(result);
			return;
		}

		D3D11_SAMPLER_DESC sampleDesciption = {};
		sampleDesciption.Filter = filtering;
		sampleDesciption.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampleDesciption.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampleDesciption.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampleDesciption.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampleDesciption.MinLOD = 0;
		sampleDesciption.MaxLOD = D3D11_FLOAT32_MAX;

		result = GraphicsManager::device->CreateSamplerState(&sampleDesciption, &samplerState);

		if (FAILED(result))
		{
			DXWIN_EXCEPTION(result);
			return;
		}
	}

	void Bind(UINT slot)
	{
		GraphicsManager::context->PSSetShaderResources(slot, 1, textureView.GetAddressOf());
		GraphicsManager::context->PSSetSamplers(slot, 1, samplerState.GetAddressOf());
	}

	static Texture Register(const std::string& path, const std::string& name)
	{
		Texture texture;
		texture.name = name;
		texture.path = "Assets/" + WSTR2STR(Settings::domain) + "/" + path;

		return texture;
	}

private:

	ComPtr<ID3D11ShaderResourceView> textureView;
	mutable ComPtr<ID3D11SamplerState> samplerState;

};

#endif // !TEXTURE_HPP