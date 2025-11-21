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

        float characterSizeFloat = static_cast<float>(characterSize);
        float leftPosXAdjust = 50.0;
        float setTextLeftPosY = yPos + (buttonDimension.y / 2 -  characterSizeFloat / 2);
        float setTextLeftPosX = xPos + leftPosXAdjust;

        //Text design
        sf::Color textColor = sf::Color::Black;
        renderText = RenderText(font, text, textColor,
        sf::Vector2f(setTextLeftPosX, setTextLeftPosY), characterSize);


        buttonMemberData.posX = xPos;
        buttonMemberData.posY = yPos;
        buttonMemberData.width = buttonDimension.x;
        buttonMemberData.height = buttonDimension.y;
        addButtonMemberData(buttonMemberData);

        strPtr = &text;
    };

    void draw(sf::RenderWindow& target) const override {
        target.draw(shape);
        renderText.draw(target);
    };
    
    
    void updateColor(sf::RenderWindow& target) override {
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(target); //Relative to current window

        if (isHovered(mousePosition)){
            sf::Color color(210, 215, 211);
            shape.setFillColor(color);
        }
        else{
            shape.setFillColor(sf::Color::White);
        }
    };

    void moveButton(float xPos, float yPos){
        shape.move(xPos, yPos);
    }

    std::string getTextObjectString() const override{return *strPtr;}

    ~UploadButton() = default;

private:
    RenderText renderText;
    const std::string* strPtr;
    sf::RectangleShape shape;
    ButtonMemberData buttonMemberData;
};