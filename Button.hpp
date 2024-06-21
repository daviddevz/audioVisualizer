#pragma once
#include <SFML/Graphics.hpp>


class Button {
protected:
    // Stores button data
    /*Create a vector of struct that stores the position (x, y) and dimension (width, height) of each button in base class using
    get function from each button class*/

    struct ButtonData {
        float posX;
        float posY;
        float width;
        float height;
    };

    std::vector<ButtonData> buttonDataVect; // Stores data for all buttons
    
public:
    Button() : windowDimension_(sf::Vector2f(0.0, 0.0)) {};
    Button(const sf::Vector2f& windowDimension) : windowDimension_(windowDimension) {
        // Upload Music Button Data
        ButtonData uploadButtonData;
        std::vector<float> uploadButtonDataVect = UploadButton::getButtonData();
        uploadButtonData.posX = uploadButtonDataVect[0];
        uploadButtonData.posY = uploadButtonDataVect[1];
        uploadButtonData.width = uploadButtonDataVect[2];
        uploadButtonData.height = uploadButtonDataVect[3];

        buttonDataVect.push_back(uploadButtonData);
    };

    // Explicit default destructor declaration to the compiler
    virtual ~Button() = default; 

    virtual void draw(sf::RenderTarget& target) const {};

    /*Create bool isHover method in base class that returns true if mouse hovers. It iterate over struct data and check if the current mouse
    position is within the button dimension. If it is, it will reutn true else false. It will take mouse position as a parameter
    Breaks the iteration when by returning true*/
    bool isHovered(const sf::Vector2i& mousePosition) const {
        // Modern C++ iteration over vector type, an iterator points to element in buttonDataVect by reference
        for(const ButtonData& ele : buttonDataVect){

        }
    };
    
    virtual bool isClicked(const sf::Vector2i& mousePosition) {return false;};

    virtual void updateColor(sf::WindowBase& target) {};

    /* 
    Refactor UpdateColor function if statement to check if ishover method returns true then update button color else default color

    Remove isClicked method and use isHover method to check if mouse was over button dimension upon click
    */

    sf::Vector2f getWindowDimension(){
        return windowDimension_;
    };

private:
    const sf::Vector2f windowDimension_;
};

class UploadButton : public Button{
public:
    UploadButton(sf::Vector2f uploadWindowDimension, const sf::Font& font, const std::string& text, const sf::Vector2f& buttonDimension, unsigned int characterSize) :
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
    };

    std::vector<float> getButtonData() {
        return {shape.getPosition().x, shape.getPosition().y, buttonDimension_.x, buttonDimension_.y};
    };

    void draw(sf::RenderTarget& target) const override {
        target.draw(shape);
        target.draw(textObject);
    };
    
    
    void updateColor(sf::WindowBase& target) override {
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(target); //Relative to current window

        if ((mousePosition.x >= shape.getPosition().x && mousePosition.x <= shape.getPosition().x + buttonDimension_.x) && 
            (mousePosition.y >= shape.getPosition().y && mousePosition.y <= shape.getPosition().y + buttonDimension_.y)){
            sf::Color color(210, 215, 211, 255); //Pumice Gray
            shape.setFillColor(color);
        }
        else{
            shape.setFillColor(sf::Color::White); //Switch to white color
        }
    };

    bool isClicked(const sf::Vector2i& mousePosition) override {
        if ((mousePosition.x >= shape.getPosition().x && mousePosition.x <= shape.getPosition().x + buttonDimension_.x) && 
            (mousePosition.y >= shape.getPosition().y && mousePosition.y <= shape.getPosition().y + buttonDimension_.y)){
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
};

class PlayButton : public Button{

};

class PauseButton : public Button{

};


