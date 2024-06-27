#pragma once
#include "HeaderFiles.hpp"

class AudioFileHandling {
public:
    AudioFileHandling(const sf::WindowHandle windowHandle) {
        openFileDir(windowHandle);
    };

    // Open file directory specific for Windows
    void openFileDir(const sf::WindowHandle windowHandle) {
        //std::cout<< "Button was clicked" << std::endl;

        OPENFILENAME ofn; // Common dialog box structure
        char szFile[MAX_PATH] = ""; // Buffer for file name how long it should be
        HWND hwnd = windowHandle; // Handle to window that owns dialog box
        HANDLE hf; // File handle
        
        SecureZeroMemory(&ofn, sizeof(ofn)); //Fills block of memory with zero
        ofn.lStructSize = sizeof(ofn); // Specify the size, in bytes, of the OPENFILENAME structure
        ofn.hwndOwner = hwnd;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile); // Size, in character, pointed to by lpstrFile
        ofn.lpstrFilter = TEXT("Music Files\0*.mp3;*.wav\0\0"); // Filter for the type of file user can select
        ofn.nFilterIndex = 1; // Preselect filter to Music Files in the dialog box
        ofn.lpstrFileTitle = NULL; // Prevent storing the title of the file
        ofn.nMaxFileTitle = 0; // Tells the dialog box that the title buffer is 0 since lpstrFileTitle is Null
        ofn.lpstrInitialDir = NULL; // Default initial directory displayed on dialog to current working directory of application
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // Bit that sets what user can do upon initilizing dialog box
        
        // Display the Open dialog box based on configuration of ofn structure 
        if (GetOpenFileName(&ofn)==TRUE) {
            std::cout<<"FILE OPENED";
            // Create handle for the file that will be used for other operation
            hf = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES) NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);

            CloseHandle(hf); // Close handle to release system resources
        }
        else {
            std::cout << "User canceled or no file selected. Error " 
            <<CommDlgExtendedError()<<std::endl;
        }
    }
    // Explicit default destructor declaration to the compiler
    ~AudioFileHandling() = default;

private:
};