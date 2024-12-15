#pragma once
/* Win32 includes*/
#include <windows.h>
#include <commdlg.h>

/* C++ includes*/
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

/* Project includes*/
#include "buttons/button.hpp"
#include "visualization/progressBar.hpp"
#include "audio processing/audioFileHandling.hpp"

/* SFML includes */
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>

class AudioVisualizer{
public:
    AudioVisualizer(){};

    void loadWindow();
    void loadFont();
    UploadButton loadUploadButton();
    ProgressBar loadProgressBar();
    void startAudioVisualizer();

    ~AudioVisualizer(){};

private:
    unsigned int windowWidth = 2000, windowHeight = 1000;
    float buttonWidth = 500, buttonHeight = 100, loadingStages = 5;
    float windowWidthFloat = static_cast<float>(windowWidth);
    float windowHeightFloat = static_cast<float>(windowHeight);
    int fontSize = 50;
    std::string uploadButtonText = "UPLOAD MUSIC", windowTitle = "Audio Visualizer";
    sf::Font font;
    sf::RenderWindow window;
};