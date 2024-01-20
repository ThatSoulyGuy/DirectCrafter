#ifndef GRAPHICS_MANAGER_HPP
#define GRAPHICS_MANAGER_HPP

#include <functional>
#include <d3d11_1.h>
#include <dxgi1_2.h>
#include <wrl.h>
#include "core/Logger.hpp"
#include "core/Settings.hpp"
#include "exception/WinException.hpp"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")

using Microsoft::WRL::ComPtr;

namespace GraphicsManager
{
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    ComPtr<IDXGISwapChain1> swapChain;
    ComPtr<ID3D11RenderTargetView> renderTargetView;

    void Initialize()
    {
        UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };

        D3D_FEATURE_LEVEL featureLevel;

        if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &device, &featureLevel, &context)))
            throw DXWIN_LAST_EXCEPTION();

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = static_cast<UINT>(Settings::window.Bounds().Width);
        swapChainDesc.Height = static_cast<UINT>(Settings::window.Bounds().Height);
        swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

        ComPtr<IDXGIDevice1> dxgiDevice;
        device.As(&dxgiDevice);

        ComPtr<IDXGIAdapter> dxgiAdapter;
        dxgiDevice->GetAdapter(&dxgiAdapter);

        ComPtr<IDXGIFactory2> dxgiFactory;
        dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

        winrt::com_ptr<IUnknown> unknownWindow = Settings::window.as<IUnknown>();

        if (FAILED(dxgiFactory->CreateSwapChainForCoreWindow(device.Get(), unknownWindow.get(), &swapChainDesc, nullptr, &swapChain)))
            throw DXWIN_LAST_EXCEPTION();

        ComPtr<ID3D11Texture2D> backBuffer;
        swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
        device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView);

        context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);

        D3D11_VIEWPORT viewport = {};
        viewport.Width = static_cast<float>(Settings::window.Bounds().Width);
        viewport.Height = static_cast<float>(Settings::window.Bounds().Height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        context->RSSetViewports(1, &viewport);
    }

    void Resize()
    {
        renderTargetView = nullptr;

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChain->GetDesc1(&swapChainDesc);

        HRESULT result = swapChain->ResizeBuffers(swapChainDesc.BufferCount, static_cast<UINT>(Settings::window.Bounds().Width), static_cast<UINT>(Settings::window.Bounds().Height), swapChainDesc.Format, swapChainDesc.Flags);

        if (FAILED(result))
            throw DXWIN_EXCEPTION(result);

        ComPtr<ID3D11Texture2D> backBuffer;

        result = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
        if (FAILED(result))
            throw DXWIN_EXCEPTION(result);
        
        result = device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView);

        if (FAILED(result))
            throw DXWIN_EXCEPTION(result);
        
        D3D11_VIEWPORT viewport = {};
        viewport.Width = static_cast<float>(Settings::window.Bounds().Width);
        viewport.Height = static_cast<float>(Settings::window.Bounds().Height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;

        context->RSSetViewports(1, &viewport);
    }

    void ClearScreen()
	{
        if (!renderTargetView)
            throw DXWIN_EXCEPTION(E_POINTER);

        FLOAT clearColor[4] = { 0.0f, 0.45f, 0.75f, 1.0f };

        context->ClearRenderTargetView(renderTargetView.Get(), clearColor);
	}

	void PreRender()
	{
		if (!swapChain)
			throw DXWIN_EXCEPTION(E_POINTER);

        context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);

        ClearScreen();
	}

    void PostRender()
    {
        swapChain->Present(1, 0);
    }
}

#endif // !GRAPHICS_MANAGER_HPP