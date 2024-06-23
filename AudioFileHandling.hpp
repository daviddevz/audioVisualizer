#pragma once
#include "HeaderFiles.hpp"

class AudioFileHandling {
public:
    AudioFileHandling(const sf::WindowHandle windowHandle) {
        openFileDir(windowHandle);
    };

    // Open file directory specific for Windows
    void openFileDir(const sf::WindowHandle windowHandle) {
        std::cout<< "Button was clicked" << std::endl;

        OPENFILENAME ofn; // Common dialog box structure
        std::string szFile; // Buffer for file name how long it should be
        HWND hwnd = windowHandle; // Handle to window that owns dialog box
        HANDLE hf; // File handle

        SecureZeroMemory(&ofn, sizeof(ofn)); //Fills block of memory with zero
        ofn.lStructSize = sizeof(ofn); // Specify the size, in bytes, of the OPENFILENAME structure
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;

        // Char that specify the file path and name for the file dialog
        LPWSTR writableWideFilePath = GetWritableWideFilePath(szFile);

        // Use the converted ANSI string for assignment
        char* ansiFilePath = convertBtwnWidecharAndANSI(writableWideFilePath);
        ofn.lpstrFile = ansiFilePath; // C-style

        // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
        // Use the contents of szFile to initialize itself.
        ofn.lpstrFile[0] = '\0';

        
        ofn.nMaxFile = sizeof(szFile); // Size, in character, pointed to by lpstrFile
        ofn.lpstrFilter = TEXT("Music Files\0*.mp3;*.wav\0\0"); // Filter for the type of file user can select
        ofn.nFilterIndex = 1; // Preselect filter to Music Files in the dialog box
        ofn.lpstrFileTitle = NULL; // Prevent storing the title of the file
        ofn.nMaxFileTitle = 0; // Tells the dialog box that the title buffer is 0 since lpstrFileTitle is Null
        ofn.lpstrInitialDir = NULL; // Default initial directory displayed on dialog to current working directory of application
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // Bit that sets what user can do upon initilizing dialog box
        
        // Display the Open dialog box based on configuration of ofn structure 
        if (GetOpenFileName(&ofn)==TRUE) { 
            hf = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES) NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);
        }
        
        // Deallocate memory after use
        delete[] ansiFilePath;
        delete[] writableWideFilePath;
    }

    // Convert std::string to writeable wide char
    LPWSTR GetWritableWideFilePath (const std::string& filePath){
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::wstring wideFilePath = converter.from_bytes(filePath);
        int wideFilePathLength = wideFilePath.length();

        // Allocate memory for the writable wide character string
        LPWSTR writableWideFilePath = new wchar_t[wideFilePathLength + 1];

        // Copy the content of wideFilePath.c_str() (including null terminator)
        std::wcscpy(writableWideFilePath, wideFilePath.c_str());
        return writableWideFilePath;
    }

    // Explicit character encoding conversion between wide characters and single-byte characters (ANSI)
    char* convertBtwnWidecharAndANSI (const LPWSTR& writableWideFilePath) const{
        // Determine required buffer size for converted string (ANSI)
        int requiredBufferSize = WideCharToMultiByte(CP_ACP, 0, writableWideFilePath, -1, NULL, 0, NULL, NULL);
        if (requiredBufferSize == 0) {
            // Handle error (e.g., failed to get required buffer size)
            delete[] writableWideFilePath;
            return nullptr;
        }

        // Allocate memory for the converted ANSI string
        char* ansiFilePath = new char[requiredBufferSize];

        // Perform conversion to ANSI encoding
        int convertedBytes = WideCharToMultiByte(CP_ACP, 0, writableWideFilePath, -1, ansiFilePath, requiredBufferSize, NULL, NULL);
        if (convertedBytes == 0) {
            // Handle error (e.g., conversion failed)
            delete[] ansiFilePath; // Free allocated memory (even on error)
            delete[] writableWideFilePath;
            return nullptr;
        }
        return ansiFilePath;
    }

    // Explicit default destructor declaration to the compiler
    ~AudioFileHandling() = default;

private:
};