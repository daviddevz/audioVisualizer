#pragma once
#include <iostream>
#include <string>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

class ProgressBar {
public:

    ProgressBar(sf::Vector2f shapePosition, const sf::Color backgroundColor, const sf::Color maskedColor,
    const sf::Vector2f& shapeDimension): backgroundColor_(backgroundColor), maskedColor_(maskedColor),
    shapeDimension_(shapeDimension), shapePosition_(shapePosition){
        designShapes();
    };

    // Shape design function
    void designShapes(){
        // Background shape design
        backgroundShape.setSize(shapeDimension_);
        backgroundShape.setPosition(shapePosition_);
        backgroundShape.setFillColor(backgroundColor_);

        // Masked shape design
        maskedShape = backgroundShape;
        maskedShape.setSize(sf::Vector2f(0.00f, shapeDimension_.y));
        maskedShape.setFillColor(maskedColor_);
    };

    // Update progressBar
    void updateProgressBar(float fractCoverage){
        // Increase the width by fractional coverage (0.00 - 1.00)
        float shapeWidth;
        if (fractCoverage < 0.0f){
            shapeWidth = 0.0f;
        }
        else if (fractCoverage > 1.0f){
            shapeWidth = shapeDimension_.x;
        }
        else{
            shapeWidth = shapeDimension_.x * fractCoverage;
        }
        maskedShape.setSize(sf::Vector2f(shapeWidth, maskedShape.getSize().y));
    };


    // Draw shape and text
    void draw(sf::RenderTarget& target){
        target.draw(backgroundShape);
        target.draw(maskedShape);
    };

    sf::Vector2f getShapePos(){
        return shapePosition_;
    }

    sf::Vector2f getShapeDim(){
        return shapeDimension_;
    }

    // Explicit default destructor declaration to the compiler
    ~ProgressBar() = default; 

private:
    float progressStages; // Section to divide up the prgress bar
    const sf::Vector2f shapeDimension_;
    sf::Vector2f shapePosition_;
    sf::RectangleShape backgroundShape, maskedShape;
    sf::Color backgroundColor_, maskedColor_;
};