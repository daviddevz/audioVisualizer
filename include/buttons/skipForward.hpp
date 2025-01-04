#pragma once

#include "button.hpp"
#include <unordered_map>

#define _USE_MATH_DEFINES
#include <cmath>

class SkipForward : public Button{
public:
    SkipForward(const sf::Vector2f windowDimension, const sf::Font& font)
    :windowDimension_(windowDimension){
        createSkipForward();

        //Text design
        /* description.setFont(font);
        description.setCharacterSize(25);
        description.setFillColor(sf::Color::White); */
    }

    void createSkipForward(){
        float xPos = (windowDimension_.x / 2) - (buttonDimension.x / 2);
        float yPos = (windowDimension_.y) - (buttonDimension.y * 5);

        // Circle part of the play button
        const float radius = buttonDimension.x;
        circle1.setRadius(radius);
        circle1.setOutlineThickness(radius * 0.25f);
        circle1.setOutlineColor(sf::Color::White);
        circle1.setFillColor(sf::Color::Black);
        circle1.setPosition(xPos + SHIFT, yPos);

        // Triangle part of the play button
        triangle1.setPointCount(3);

        struct TriVert{
            sf::Vector2f v1, v2, v3;
        };

        TriVert triVert;
        triVert.v1 = {0.55f * radius, 0.0f};

        triVert.v2 = {0.55f * radius * (1.0f - sinf(M_PI/3.0f) * sqrtf(3)),
        0.55f * radius * sinf(M_PI/3.0f)};

        triVert.v3 = {0.55f * radius * (1.0f - sinf(M_PI/3.0f) * sqrtf(3)),
        -0.55f * radius * sinf(M_PI/3.0f)};

        triangle1.setPoint(0, triVert.v1);
        triangle1.setPoint(1, triVert.v2);
        triangle1.setPoint(2, triVert.v3);
        triangle1.setFillColor(sf::Color::White);
        triangle1.setPosition(xPos + SHIFT + radius , yPos + radius);
        triangle1.move(-0.2f * radius, 0.0f);

        // Rectangle Setup
        rectangle1.setPosition(xPos + SHIFT + radius, yPos + radius);
        rectangle1.setSize(sf::Vector2f(0.30f * radius, 2.0f * 0.55f * radius * sinf(M_PI/3.0f)));
        rectangle1.setFillColor(sf::Color::White);
        rectangle1.move(0.35f * radius, -0.55f * radius * sinf(M_PI/3.0f));

        buttonMemberData = {xPos + SHIFT, yPos, radius * 2.0f, radius * 2.0f};
    }

    void draw(sf::RenderTarget& target) const override{
        target.draw(circle1);
        target.draw(triangle1);
        target.draw(rectangle1);
    }

    //To Do: Fix update color for backward button
    void updateColor(sf::RenderWindow& window) override{
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        addButtonMemberData(buttonMemberData);

        if (isHovered(mousePosition)){
            circle1.setFillColor(sf::Color(210, 215, 211, 100));
        }
        else{
            circle1.setFillColor(sf::Color(sf::Color::Black)); 
        }
    }

    bool isClicked(const sf::RenderWindow& target) override{
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(target);

        addButtonMemberData(buttonMemberData);

        if (isHovered(mousePosition)) {
            return true;
        }
        return false;
    }

    ~SkipForward() = default;
    
private:
    const float SHIFT = 100.0f;
    //sf::Text description;
    sf::RectangleShape rectangle1, rectangle2;
    sf::CircleShape circle1, circle2;
    sf::ConvexShape triangle1, triangle2;
    sf::Vector2f buttonDimension = {30.0f, 30.0f};
    const sf::Vector2f windowDimension_;
    ButtonMemberData buttonMemberData;
};