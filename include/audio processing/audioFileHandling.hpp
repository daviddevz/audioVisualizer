#pragma once
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <string>
#include <memory>
#include "audioVisualizer.hpp"

class AudioFileHandling {
public:
    std::unique_ptr<char[]> filePath = std::make_unique<char[]>(MAX_PATH); // Path to the selected file for playback;
    AudioFileHandling(){}

    // Open file directory specific for Windows
    void openFileDir(const sf::WindowHandle windowHandle){
        #ifdef _WIN32
            OPENFILENAMEW ofn; // Common dialog box structure
            wchar_t szFile[MAX_PATH] = L""; // Buffer for file name how long it should be
            HWND hwnd = windowHandle; // Handle to window that owns dialog box
            HANDLE hf; // File handle

            SecureZeroMemory(&ofn, sizeof(ofn)); //Fills block of memory with zero
            ofn.lStructSize = sizeof(ofn); // Specify the size, in bytes, of the OPENFILENAME structure
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = MAX_PATH; // Size, in character, pointed to by lpstrFile
            ofn.lpstrFilter = L"Music Files\0*.mp3;*.wav\0\0"; // Filter for the type of file user can select
            ofn.nFilterIndex = 1; // Preselect filter to Music Files in the dialog box
            ofn.lpstrFileTitle = NULL; // Prevent storing the title of the file
            ofn.nMaxFileTitle = 0; // Tells the dialog box that the title buffer is 0 since lpstrFileTitle is Null
            ofn.lpstrInitialDir = nullptr; // Default initial directory displayed on dialog to current working directory of application
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR; // Bit that sets what user can do upon initializing dialog box
            
            // Display the Open dialog box based on configuration of ofn structure 
            if(GetOpenFileNameW(&ofn) == TRUE) {
                // Convert the wide character string to narrow character string
                int size = WideCharToMultiByte(CP_UTF8, 0, ofn.lpstrFile, -1, NULL, 0, NULL, NULL);
                //std::unique_ptr<char[]> narrowStr = std::make_unique<char[]>(size);
                std::string narrowStr(size, '\0');

                WideCharToMultiByte(CP_UTF8, 0, ofn.lpstrFile, -1, narrowStr.data(), size, NULL, NULL);

                // Create handle for the file that will be used for other operation
                std::wstring wideStr = narrowToWide(narrowStr);
                hf = CreateFileW(wideStr.c_str(), GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES) NULL,
                OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);

                strcpy(filePath.get(), narrowStr.data()); // Copy narrowStr to filePath

                CloseHandle(hf); // Close handle to release system resources

        #endif
        }
    }

    std::wstring narrowToWide(const std::string& str){
        std::wstring wideStr;
        wideStr.resize(str.length());
        std::mbstowcs(&wideStr[0], str.c_str(), str.length());
        return wideStr;
    }

    ~AudioFileHandling(){}
};