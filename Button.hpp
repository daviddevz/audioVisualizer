#pragma once
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
    Button() : windowDimension_(sf::Vector2f(0.0, 0.0)) {};
    Button(const sf::Vector2f& windowDimension) : windowDimension_(windowDimension) {
    };

    // Explicit default destructor declaration to the compiler
    virtual ~Button() = default; 

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

    virtual bool isClicked(const sf::WindowBase& target) const{return false;};

    virtual void updateColor(sf::WindowBase& target) {};

    sf::Vector2f getWindowDimension(){
        return windowDimension_;
    };

private:
    const sf::Vector2f windowDimension_;
};

class UploadButton : public Button{
public:
    UploadButton(sf::Vector2f uploadWindowDimension, const sf::Font& font,
    const std::string& text, const sf::Vector2f& buttonDimension, unsigned int characterSize) :
    Button(uploadWindowDimension), buttonDimension_(buttonDimension) {
        //Button design
        float centerX = Button::getWindowDimension().x / 2; // Window X center
        float centerY = Button::getWindowDimension().y / 2; // Window Y center

        shape.setSize(buttonDimension);

        float setLeftPosX = centerX - buttonDimension_.x / 2; // Recangle X position offset
        float setLeftPosY = centerY - buttonDimension_.y / 2; // Recangle Y position offset
        shape.setPosition(setLeftPosX, setLeftPosY); // Set rectangle post

        //Text design
        textObject.setFont(font);
        textObject.setString(text);
        textObject.setCharacterSize(characterSize);
        textObject.setFillColor(sf::Color::Black);
        
        float characterSizeFloat = static_cast<float>(characterSize);
        float leftPosXAdjust = 50.0;
        float setTextLeftPosY = setLeftPosY + (buttonDimension.y / 2 -  characterSizeFloat / 2);
        float setTextLeftPosX = setLeftPosX + leftPosXAdjust;

        textObject.setPosition(setTextLeftPosX, setTextLeftPosY);
        textObject.setStyle(sf::Text::Bold);

        // Stage member data of upload button and add button data to base class buttonDataVect
        uploadButtonMemberData.posX = setLeftPosX;
        uploadButtonMemberData.posY = setLeftPosY;
        uploadButtonMemberData.width = buttonDimension_.x;
        uploadButtonMemberData.height = buttonDimension_.y;
        Button::addButtonMemberData(uploadButtonMemberData);
    };

    void draw(sf::RenderTarget& target) const {
        target.draw(shape);
        target.draw(textObject);
    };
    
    
    void updateColor(sf::WindowBase& target) override {
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(target); //Relative to current window

        if (Button::isHovered(mousePosition)){
            sf::Color color(210, 215, 211, 255); //Pumice Gray
            shape.setFillColor(color);
        }
        else{
            shape.setFillColor(sf::Color::White); //Switch to white color
        }
    };

    bool isClicked(const sf::WindowBase& target) const override {
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(target); //Relative to current window

        if (Button::isHovered(mousePosition)) {
            return true;
        }
        else {
            return false;
        }
    }

    void openFileDir() {
        std::cout<< "Button was clicked" << std::endl;
    }

private:
    // Member variables for button properties (text, shape, colors)
    sf::Text textObject;
    sf::RectangleShape shape;
    const sf::Vector2f buttonDimension_;
    Button::ButtonMemberData uploadButtonMemberData;
};

class PlayButton : public Button{

};

class PauseButton : public Button{

};


