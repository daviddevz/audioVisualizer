#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <memory>

class Scene{
public:
    Scene(){};
    
    virtual void load(sf::RenderWindow& window) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    /* virtual sf::Color getClearColor(){
        return sf::Color::Black;
    } */

    virtual void clickActions(sf::RenderWindow& window) = 0;
    virtual void cursorActions(sf::RenderWindow& window, sf::RenderTarget& target) = 0;
    
    virtual bool shouldMoveToNextScene() = 0;

    /* virtual std::string getNextSceneId(std::vector<std::string>& sceneIds, std::string& scene){
        return "";
    } */

    virtual const std::string getFilePath(){return "";};
    virtual const std::string getTypeOfVisual(){return "";}

    void loadFont(sf::Font& font){
        if(!font.loadFromFile("..\\font\\ARIAL.TTF")){
            throw std::runtime_error("Failed to load font");
        }
    }

    virtual void setFilePath(const std::string& filePath_){};
       
    virtual ~Scene() = default;

private:
    
};