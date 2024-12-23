#pragma once

class Scene{
public:
    Scene(){};
    
    virtual void clickActions(sf::WindowBase& window) = 0;
    virtual void cursorActions(sf::WindowBase& window) = 0;
    virtual void load(sf::WindowBase& window) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual const std::string getFilePath(){
        return "";
    }
    virtual void setFilePath(const std::string& filePath_){}

    virtual sf::Color getClearColor(){
        return sf::Color::Black;
    }

    virtual bool shouldMoveToNextScene() = 0;
    
    virtual std::string getNextSceneId(){
        return "";
    }

    virtual ~Scene() = default;
};