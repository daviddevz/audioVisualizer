#include <SFML/Graphics.hpp>

class Button {
public:
    Button() : windowDimension_(sf::Vector2f(0.0, 0.0)) {};
    Button(const sf::Vector2f& windowDimension) : windowDimension_(windowDimension) {};

    virtual void draw(sf::RenderTarget& target) const {};
    
    virtual bool isClicked(const sf::Vector2f& mousePosition) const {return false;};

    // Member functions for handling button state (normal, hover, clicked)
    virtual void updateColor(sf::Color color) const {};

    float getXWindowDimension(){
        return windowDimension_.x;
    };

    float getYWindowDimension(){
        return windowDimension_.y;
    }

private:
    const sf::Vector2f windowDimension_;
};

class UploadButton : public Button{
public:
    UploadButton(sf::Vector2f uploadWindowDimension, const sf::Font& font, const std::string& text, const sf::Vector2f& buttonDimension, unsigned int characterSize) :
    Button(uploadWindowDimension) {
        //Button design
        float centerX = Button::getXWindowDimension() / 2; // Window X center
        float centerY = Button::getYWindowDimension() / 2; // Window Y center

        shape.setSize(buttonDimension);

        float setLeftPosX = centerX - buttonDimension.x / 2; // Recangle X position offset
        float setLeftPosY = centerY - buttonDimension.y / 2; // Recangle Y position offset
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

    void draw(sf::RenderTarget& target) const override {
        target.draw(shape);
        target.draw(textObject);
    };

    void updateColor(sf::Color color) const override {

    };

private:
    // Member variables for button properties (text, shape, colors)
    sf::Text textObject;
    sf::RectangleShape shape;
};

class PlayButton : public Button{

};

class PauseButton : public Button{

};


