// Written by David I. 2024
// Button to upload music
#pragma once
#include "button.hpp"

class UploadButton : public Button{
public:
    UploadButton(sf::Vector2f windowDimension, const sf::Font& font,
    const std::string& text, const sf::Vector2f& buttonDimension, unsigned int characterSize){
        //Button design
        float xPos = (windowDimension.x / 2) - (buttonDimension.x / 2); // button X center
        float yPos = (windowDimension.y / 2) - (buttonDimension.y / 2); // button Y center

        shape.setSize(buttonDimension);
        
        shape.setPosition(xPos, yPos); // Set rectangle position

        //Text design
        textObject.setFont(font);
        textObject.setString(text);
        textObject.setCharacterSize(characterSize);
        textObject.setFillColor(sf::Color::Black);
        
        float characterSizeFloat = static_cast<float>(characterSize);
        float leftPosXAdjust = 50.0;
        float setTextLeftPosY = yPos + (buttonDimension.y / 2 -  characterSizeFloat / 2);
        float setTextLeftPosX = xPos + leftPosXAdjust;

        textObject.setPosition(setTextLeftPosX, setTextLeftPosY);
        textObject.setStyle(sf::Text::Bold);

        buttonMemberData.posX = xPos;
        buttonMemberData.posY = yPos;
        buttonMemberData.width = buttonDimension.x;
        buttonMemberData.height = buttonDimension.y;
        addButtonMemberData(buttonMemberData);
    };

    void draw(sf::RenderTarget& target) const override {
        target.draw(shape);
        target.draw(textObject);
    };
    
    
    void updateColor(sf::RenderWindow& target) override {
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(target); //Relative to current window

        if (isHovered(mousePosition)){
            sf::Color color(210, 215, 211); //Pumice Gray
            shape.setFillColor(color);
        }
        else{
            shape.setFillColor(sf::Color::White); //Switch to white color
        }
    };

    std::string getTextObjectString() const override{
        return textObject.getString();
    }

    ~UploadButton(){}

private:
    sf::Text textObject;
    sf::RectangleShape shape;
    ButtonMemberData buttonMemberData;
};