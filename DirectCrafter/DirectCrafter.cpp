#define NOMINMAX

#include "core/GraphicsManager.hpp"
#include "core/InputManager.hpp"
#include "entity/Player.hpp"
#include "render/Renderer.hpp"
#include "render/ShaderManager.hpp"
#include "render/TextureManager.hpp"
#include "exception/WinException.hpp"
#include "exception/MessageBox.hpp"

struct DirectCrafter : implements<DirectCrafter, IFrameworkViewSource, IFrameworkView>
{
    Player player;
    Mesh mesh;

    IFrameworkView CreateView()
    {
        return *this;
    }

    void Initialize(CoreApplicationView const&)
    {
        
    }

    void PostInitialize()
    {
        Logger_Init();

        Logger_WriteConsole("DirectCrafter is starting up...", LogLevel::INFO);

        try
        {
            GraphicsManager::Initialize();

            InputManager::Initialize();

            ShaderManager::Register(Shader::Register(L"Shaders/Default", "default"));
            TextureManager::Register(Texture::Register("Textures/block.png", "test"));

            player.Initialize({ 0.0f, 0.0f, -5.0f }, {0.0f, 0.0f, 0.0f});

            mesh = Mesh::Register("test", {}, {}, "default");
            mesh.GenerateSquare();
            Renderer::Register(mesh);
        }
        DX_EXEPTION_CATCH
	}

    void Load(hstring const&)
    {

    }

    void Uninitialize()
    {
        Logger_CleanUp();
    }

    void Run()
    {
        CoreWindow window = CoreWindow::GetForCurrentThread();
        window.ResizeCompleted([&](auto&&, auto&&) { WindowResized(window); });
        window.Activate();
        Settings::window = window;

        PostInitialize();

        CoreDispatcher dispatcher = window.Dispatcher();

        bool isRunning = true;

        window.Closed([&](auto&&, auto&&) 
        {
            isRunning = false;
        });

        while (isRunning)
        {
            dispatcher.ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

            Render();
        }
    }

    void Render()
    {
        try
        {
            Settings::window = CoreWindow::GetForCurrentThread();

            GraphicsManager::PreRender();

            InputManager::Update();

            player.Update();
            Renderer::Render(player.camera);

            GraphicsManager::PostRender();
        }
        DX_EXEPTION_CATCH
    }

    void WindowResized(CoreWindow const&)
	{
        GraphicsManager::Resize();
	}

    void SetWindow(CoreWindow const & window)
    {
        
    }
};

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    CoreApplication::Run(make<DirectCrafter>());
}
