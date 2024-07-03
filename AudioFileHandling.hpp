#pragma once
#include "HeaderFiles.hpp"

class AudioFileHandling {
public:
    AudioFileHandling(const sf::WindowHandle windowHandle) {
        openFileDir(windowHandle);
        
    };

    // Open file directory specific for Windows
    void openFileDir(const sf::WindowHandle windowHandle) {
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
            // Create handle for the file that will be used for other operation
            hf = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES) NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);
            
            filePath = ofn.lpstrFile; // Defines filePath
            CloseHandle(hf); // Close handle to release system resources
        }
    }

    // Returns 0 if it is a wave file and 1 if it is an mp3
    // SFML can only read wav file so this function will tell us when a conversion is required
    int checkAudioFileType() const{
        if(getFileExtension(filePath) == ".mp3"){
            return 1;
        };
        return 0;
    }

    // Return file extension of the music file
    std::string getFileExtension(const char* filePath) const{
        if (filePath == nullptr) {
            return ""; // Handle null pointer case
        }

        // Find the last dot (.) in the filename
        const char* dot_pos = strrchr(filePath, '.');

        // If no dot is found, return an empty string
        if (dot_pos == nullptr) {
            return "";
        }

        // Extract the extension from the dot position to the end
        std::string extension(dot_pos);

        // Convert the extension to lowercase for case-insensitive comparison (optional)
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);        
        return extension;
    }

    // Explicit default destructor declaration to the compiler
    ~AudioFileHandling() = default;

private:
    char* filePath;
};