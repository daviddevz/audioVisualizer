#pragma once
#include <iostream>
#include <string>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "visualization/renderText.hpp"


// A struct to store member data of button object 
struct ButtonMemberData {
    float posX; // X position of button
    float posY; // Y position of button
    float width; // width of button
    float height; // height of button
};

class Button {
public:
    Button(){};

    // Add member data of each button to ButtonMemberData vector 
    void addButtonMemberData(const ButtonMemberData& buttonMemberData_) {
        buttonMemberData = buttonMemberData_;
    };

    /* Iterate over struct vector and check if the current mouse position is within
    the button dimension. Returns true if user hovers over button else false*/
    bool isHovered(const sf::Vector2i& mousePosition) const {
        return (mousePosition.x >= static_cast<int>(buttonMemberData.posX)) &&
                (mousePosition.x <= static_cast<int>(buttonMemberData.posX) + static_cast<int>(buttonMemberData.width)) &&
                (mousePosition.y >= static_cast<int>(buttonMemberData.posY)) &&
                (mousePosition.y <= static_cast<int>(buttonMemberData.posY) + static_cast<int>(buttonMemberData.height));
    };

    virtual void draw(sf::RenderWindow& target) const = 0;

    virtual std::string getTextObjectString() const {return "";}

    virtual void updateColor(sf::RenderWindow& target) = 0;

    virtual bool isClicked(const sf::RenderWindow& target) {
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(target); //Relative to current window

        if (isHovered(mousePosition)) {
            return true;
        }
        return false;
    };

    virtual ~Button() = default; 

private:
    ButtonMemberData buttonMemberData; 
};

