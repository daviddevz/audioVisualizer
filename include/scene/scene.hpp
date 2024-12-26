#pragma once
#include "SFML/Window.hpp"

class Scene{
public:
    Scene(){};
    
    virtual void load(sf::RenderWindow& window) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    virtual sf::Color getClearColor(){
        return sf::Color::Black;
    }

    virtual void clickActions(sf::RenderWindow& window) = 0;
    virtual void cursorActions(sf::RenderWindow& window, sf::RenderTarget& target) = 0;
    
    virtual bool shouldMoveToNextScene() = 0;

    virtual std::string getNextSceneId(){
        return "";
    }

    virtual const std::string getFilePath(){
        return "";
    }

    virtual void setFilePath(const std::string& filePath_){}    
    virtual ~Scene() = default;
};