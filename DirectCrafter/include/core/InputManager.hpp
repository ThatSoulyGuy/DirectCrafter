#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <unordered_map>
#include <chrono>
#include "core/Settings.hpp"

using winrt::Windows::System::VirtualKey;
using winrt::Windows::UI::Input::PointerUpdateKind;
using winrt::Windows::Foundation::Point;

enum class KeyState
{
    PRESSED,
    RELEASED
};

namespace InputManager
{
    bool isCursorLocked = false;
    std::unordered_map<VirtualKey, bool> keyState;
    std::unordered_map<PointerUpdateKind, bool> mouseButtonState;
    float deltaTime = 0.0f;
    Point mousePosition;

    void Initialize(CoreWindow const& window = Settings::window)
    {
        window.KeyDown({ [](auto&&, auto&& args)
        {
            keyState[args.VirtualKey()] = true;
        } });

        window.KeyUp({ [](auto&&, auto&& args)
        {
            keyState[args.VirtualKey()] = false;
        } });

        window.PointerPressed({ [](auto&&, auto&& args)
        {
            auto properties = args.CurrentPoint().Properties();
            mouseButtonState[properties.PointerUpdateKind()] = true;
        } });

        window.PointerReleased({ [](auto&&, auto&& args)
        {
            auto properties = args.CurrentPoint().Properties();
            mouseButtonState[properties.PointerUpdateKind()] = false;
        } });

        window.PointerMoved({ [](auto&&, auto&& args)
        {
            mousePosition = args.CurrentPoint().Position();
        } });
    }

    bool GetKey(VirtualKey key, KeyState state)
    {
        if (state == KeyState::PRESSED)
            return keyState[key];
        else
            return !keyState[key];
    }

    bool GetMouseButton(PointerUpdateKind button, KeyState state)
    {
        if (state == KeyState::PRESSED)
            return mouseButtonState[button];
        else
            return !mouseButtonState[button];
    }

    Point GetMousePosition()
    {
        return mousePosition;
    }

    void Update()
    {
        static std::chrono::high_resolution_clock::time_point lastFrameTime = std::chrono::high_resolution_clock::now();
        auto currentFrameTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentFrameTime - lastFrameTime).count();
        lastFrameTime = currentFrameTime;

        if (InputManager::isCursorLocked)
        {
            auto window = CoreWindow::GetForCurrentThread();
            auto center = Point(10, 10);
            window.PointerPosition(center);
        }
    }
    
    void SetCursorVisibility(bool visible, CoreWindow const& window = Settings::window)
    {
        if (visible)
        {
            auto defaultCursor = CoreCursor(CoreCursorType::Arrow, 0);
            window.PointerCursor(defaultCursor);
        }
        else
        {
            window.PointerCursor(nullptr);
        }
    }

    void SetCursorLockMode(bool locked, CoreWindow const& window = Settings::window)
    {
        //SetCursorVisibility(!locked, window);
        InputManager::isCursorLocked = locked;

        if (locked)
        {
            auto center = Point(window.Bounds().Width / 2, window.Bounds().Height / 2);
            window.PointerPosition(center);
        }
    }
}

#endif // !INPUT_MANAGER_HPP