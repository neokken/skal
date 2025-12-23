//
// Created by okker on 21/12/2025.
//

#include "file_dialogue.h"

#include <windows.h>
#include <commdlg.h>
#include <filesystem>
#include <shlobj.h>


std::string editor::FileDialogue::OpenFile(const char *filter)
{
    OPENFILENAMEA ofn;
    CHAR fileName[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    ofn.lpstrTitle = "Select a File";

    if (GetOpenFileNameA(&ofn))
    {
        return std::string(fileName);
    }
    return {};
}

std::string editor::FileDialogue::SaveFile(const char *filter)
{
    OPENFILENAMEA ofn;
    CHAR fileName[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    ofn.lpstrDefExt = "file";
    ofn.lpstrTitle = "Save File As";

    if (GetSaveFileNameA(&ofn))
    {
        return std::string(fileName);
    }
    return {};
}

std::string editor::FileDialogue::OpenFolder()
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) return {};

    IFileDialog* pfd = nullptr;
    hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if (FAILED(hr))
    {
        CoUninitialize();
        return {};
    }

    DWORD dwOptions = 0;
    hr = pfd->GetOptions(&dwOptions);
    if (SUCCEEDED(hr))
    {
        hr = pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
    }

    if (SUCCEEDED(hr))
    {
        hr = pfd->SetTitle(L"Select a Folder");
    }

    if (SUCCEEDED(hr))
    {
        hr = pfd->Show(nullptr);
    }

    std::string result;
    if (SUCCEEDED(hr))
    {
        IShellItem* psi = nullptr;
        hr = pfd->GetResult(&psi);
        if (SUCCEEDED(hr))
        {
            PWSTR pszPath = nullptr;
            hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
            if (SUCCEEDED(hr))
            {
                result = std::filesystem::path(pszPath).string();
                CoTaskMemFree(pszPath);
            }
            psi->Release();
        }
    }

    pfd->Release();
    CoUninitialize();
    return result;
}