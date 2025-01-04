#pragma once
#include "SFML/Window.hpp"
#include "visualization/progressBar.hpp"
#include "scene/sceneManager.hpp"


class AudioVisualizer{
public:
    AudioVisualizer(){};

    void loadWindow();
    //void loadFont();
    //UploadButton loadUploadButton();
    ProgressBar loadProgressBar();
    void startAudioVisualizer();

    ~AudioVisualizer(){};

private:
    unsigned int windowWidth = 2000, windowHeight = 1000;
    std::string windowTitle = "Audio Visualizer", sceneId;
    sf::RenderWindow window;
};