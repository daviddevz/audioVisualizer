#pragma once
#include <iostream>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


class Button {
protected:
    // A struct to store member data of button object 
    struct ButtonMemberData {
        float posX; // X position of button
        float posY; // Y position of button
        float width; // width of button
        float height; // height of button
    };
    
    // vector of member data for each button
    std::vector<ButtonMemberData> buttonMemberDataVect; 

public:
    //bool clearButton = false;
    
    //Button() : windowDimension_(sf::Vector2f(0.0, 0.0)) {};
    Button(){};

    // Add member data of each button to ButtonMemberData vector 
    void addButtonMemberData(const ButtonMemberData& buttonMemberData) {
        buttonMemberDataVect.push_back(buttonMemberData);
    };

    /* Iterate over struct vector and check if the current mouse position is within
    the button dimension. Returns true if user hovers over button else false*/
    bool isHovered(const sf::Vector2i& mousePosition) const {
        /* Modern C++ iteration over vector type, an iterator points to element in 
        buttonMemberDataVect by reference*/
        for(const ButtonMemberData& button : buttonMemberDataVect){
            if ((mousePosition.x >= static_cast<int>(button.posX) && mousePosition.x <=
            (static_cast<int>(button.posX) + static_cast<int>(button.width))) &&
            (mousePosition.y >= static_cast<int>(button.posY) && mousePosition.y <=
            (static_cast<int>(button.posY) + static_cast<int>(button.height)))) {
                return true;
            }
        }
        return false;
    };

    virtual void draw(sf::RenderTarget& target) const = 0;

    virtual std::string getTextObjectString() const {
        return "";
    }

    virtual void updateColor(sf::WindowBase& target) = 0;

    // Check if button was clicked
    virtual bool isClicked(const sf::WindowBase& target) const{
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(target); //Relative to current window

        if (isHovered(mousePosition)) {
            return true;
        }
        else {
            return false;
        }
        return false;
    };

    // Explicit default destructor declaration to the compiler
    virtual ~Button() = default; 

private:
};



class PlayButton : public Button{

};

class PauseButton : public Button{

};


