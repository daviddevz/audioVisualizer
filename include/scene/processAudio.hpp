// Written by David I. 2024
// Scene to process music

#pragma once
#include "SFML/Window.hpp"
#include "scene/scene.hpp"

class ProcessAudio : public Scene{
public:
    ProcessAudio(){};

    void clickActions(sf::WindowBase& window){

    };
    void cursorActions(sf::WindowBase& window){

    };
    void load(sf::WindowBase& window){

    };
    void render(sf::RenderWindow& window){

    };
    bool shouldMoveToNextScene(){

    };

    void setFilePath(const std::string& filePath_) override {
        filePath = filePath_;
        //std::cout <<"Process Audio " << filePath << std::endl;
    }

    ~ProcessAudio(){};

private:
    std::string filePath;

};