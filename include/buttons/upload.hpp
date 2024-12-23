// Written by David I. 2024
// Button to upload music
#pragma once
#include "button.hpp"

class UploadButton : public Button{
public:
    UploadButton(sf::Vector2f windowDimension, const sf::Font& font,
    const std::string& text, const sf::Vector2f& buttonDimension, unsigned int characterSize):
    buttonDimension_(buttonDimension) {
        //Button design
        float centerX = (windowDimension.x / 2) - (buttonDimension_.x / 2); // button X center
        float centerY = (windowDimension.y / 2) - (buttonDimension_.y / 2); // button Y center

        shape.setSize(buttonDimension);
        
        shape.setPosition(centerX, centerY); // Set rectangle position

        //Text design
        textObject.setFont(font);
        textObject.setString(text);
        textObject.setCharacterSize(characterSize);
        textObject.setFillColor(sf::Color::Black);
        
        float characterSizeFloat = static_cast<float>(characterSize);
        float leftPosXAdjust = 50.0;
        float setTextLeftPosY = centerY + (buttonDimension.y / 2 -  characterSizeFloat / 2);
        float setTextLeftPosX = centerX + leftPosXAdjust;

        textObject.setPosition(setTextLeftPosX, setTextLeftPosY);
        textObject.setStyle(sf::Text::Bold);

        // Stage member data of upload button and add button data to base class buttonDataVect
        uploadButtonMemberData.posX = centerX;
        uploadButtonMemberData.posY = centerY;
        uploadButtonMemberData.width = buttonDimension_.x;
        uploadButtonMemberData.height = buttonDimension_.y;
        Button::addButtonMemberData(uploadButtonMemberData);
    };

    void draw(sf::RenderTarget& target) const override {
        target.draw(shape);
        target.draw(textObject);
    };
    
    
    void updateColor(sf::WindowBase& target) override {
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(target); //Relative to current window

        if (Button::isHovered(mousePosition)){
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


private:
    sf::Text textObject;
    sf::RectangleShape shape;
    const sf::Vector2f buttonDimension_;
    Button::ButtonMemberData uploadButtonMemberData;
};