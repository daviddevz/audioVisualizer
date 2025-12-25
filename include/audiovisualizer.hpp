#pragma once
#include "SFML/Window.hpp"
#include "scene/sceneManager.hpp"
#include "SFML/System/Clock.hpp"


class AudioVisualizer{
public:
    AudioVisualizer() = default;;
    void loadWindow();
    void startAudioVisualizer();
    ~AudioVisualizer() = default;

private:
    unsigned int windowWidth = 2000, windowHeight = 1000, fps = 60;
    std::string windowTitle = "Audio Visualizer", sceneId;
    sf::RenderWindow window;
};