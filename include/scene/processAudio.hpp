// Written by David I. 2024
// Scene to process music

#pragma once
#include "scene/scene.hpp"
#include "visualization/progressBar.hpp"

class ProcessAudio : public Scene{
public:
    ProgressBar* progressBar;

    ProcessAudio(){};

    void load(sf::RenderWindow& window) override{

    };
    void render(sf::RenderWindow& window) override{

    };
    void clickActions(sf::RenderWindow& window) override{

    };
    void cursorActions(sf::RenderWindow& window, sf::RenderTarget& target) override{

    };
    
    bool shouldMoveToNextScene() override{
        return true;
    };

    void setFilePath(const std::string& filePath_) override {
        filePath = filePath_;
    }

    ~ProcessAudio(){
        delete progressBar;
    };

private:
    std::string filePath;
};