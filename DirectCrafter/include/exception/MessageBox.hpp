#ifndef MESSAGE_BOX_HPP
#define MESSAGE_BOX_HPP

#include <Windows.h>
#include <winrt/Windows.UI.Popups.h>
#include <winrt/Windows.Foundation.h>
#include <string>

using namespace winrt;
using namespace winrt::Windows::UI::Popups;

#define MessageBox MessageBoxA

int MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
    std::string textStr(lpText ? lpText : "");
    std::string captionStr(lpCaption ? lpCaption : "");

    winrt::hstring text = winrt::to_hstring(textStr);
    winrt::hstring caption = winrt::to_hstring(captionStr);

    MessageDialog dialog(text, caption);

    if (uType == MB_OK)
        dialog.Commands().Append(UICommand(L"OK", nullptr));
    else if (uType == MB_OKCANCEL)
    {
        dialog.Commands().Append(UICommand(L"OK", nullptr));
        dialog.Commands().Append(UICommand(L"Cancel", nullptr));
        dialog.CancelCommandIndex(1);
    }

    dialog.ShowAsync();
    
    return 0;
}

#endif // !MESSAGE_BOX_HPP