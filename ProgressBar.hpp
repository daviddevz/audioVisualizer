#pragma once
#include "HeaderFiles.hpp"

class ProgressBar {
public:
    bool clearProgressBar;

    ProgressBar(float stages, sf::Vector2f windowDimension,
    const sf::Vector2f& shapeDimension) : progressStages(stages), shapeDimension_(shapeDimension),
    windowDimension_(windowDimension){
        clearProgressBar = false;
    };

    // Shape design function
    void shapeDesign(const sf::Color backgroundColor = sf::Color::Black, const sf::Color maskedColor = sf::Color::White){
        float centerX = windowDimension_.x / 2; // Window X center
        float centerY = windowDimension_.y / 2; // Window Y center

        // Background shape design
        backgroundShape.setSize(shapeDimension_);

        float setLeftPosX = centerX - shapeDimension_.x / 2; // Recangle X position offset
        float setLeftPosY = centerY - shapeDimension_.y / 2; // Recangle Y position offset
        backgroundShape.setPosition(setLeftPosX, setLeftPosY); // Set rectangle position
        backgroundShape.setFillColor(backgroundColor);

        // Masked shape design
        maskedShape = backgroundShape;
        maskedShape.setSize(sf::Vector2f(shapeDimension_.x/progressStages, shapeDimension_.y));
        maskedShape.setFillColor(maskedColor);
        return;
    };

    // Update progressBar
    void updateProgressBar(float currentStage, std::string stageDescription){
        textObject.setString(stageDescription);

        // Get current size and fractionalize it by the current stage
        float shapeWidth = maskedShape.getSize().x * currentStage;  

        maskedShape.setSize(sf::Vector2f(shapeWidth, maskedShape.getSize().y));
        return;
    };

    // Setup text object
    void setText(const sf::Font& font, unsigned int characterSize){
        textObject.setFont(font);
        textObject.setCharacterSize(characterSize);
        textObject.setFillColor(sf::Color::Black);

        float setTextLeftPosY = backgroundShape.getPosition().y - 50.0; // Adjust text to be above progress bar
        float setTextLeftPosX = backgroundShape.getPosition().x;

        textObject.setPosition(setTextLeftPosX, setTextLeftPosY);
        //textObject.setPosition(0, 0);
        textObject.setStyle(sf::Text::Bold);
        return;
    };

    // Draw shape and text
    void draw(sf::RenderTarget& target) const{
        target.draw(backgroundShape);
        target.draw(maskedShape);
        target.draw(textObject);
    };

    // Explicit default destructor declaration to the compiler
    ~ProgressBar() = default; 

private:
    float progressStages; // Section to divide up the prgress bar
    const sf::Vector2f shapeDimension_;
    sf::Vector2f windowDimension_;
    sf::RectangleShape backgroundShape;
    sf::RectangleShape maskedShape;
    sf::Text textObject;
};