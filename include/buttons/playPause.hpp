#pragma once

#include "button.hpp"
#include <unordered_map>

#define _USE_MATH_DEFINES
#include <cmath>

class PlayPause : public Button{
public:
    int buttonType = 0;
    PlayPause(const sf::Vector2f windowDimension, const sf::Font& font)
    :windowDimension_(windowDimension), font_(font) {
        createPlay();
        createPause();

        //Text design
        description.setFont(font_);
        description.setCharacterSize(25);
        description.setFillColor(sf::Color::White);
    }

    void createPlay(){
        float xPos = (windowDimension_.x / 2) - (buttonDimension.x / 2);
        float yPos = (windowDimension_.y) - (buttonDimension.y * 5);

        // Circle part of the play button
        const float radius = buttonDimension.x;
        circle.setRadius(radius);
        circle.setOutlineThickness(radius * 0.25f);
        circle.setOutlineColor(sf::Color::White);
        circle.setFillColor(sf::Color::Black);
        circle.setPosition(xPos, yPos);

        // Triangle part of the play button
        triangle.setPointCount(3);

        // Triangle vertices
        /*
            The following formulas were used to determine the vertices
            of triangle inscribed in the circle shape:
            Law of Sines: a/sinA = b/sinB = c/sinC

            For any triangle inscribed in a circle
            2R = a/sinA where R is the radius

            For an equilateral triangle
            A = B = C and a = b = c

            Using 45-45-90, 30-60-90 triangle rules and slope formula (y2-y1) = m(x2-x1)
            we get the following vertices assuming a starting position of (0, 0)
            (x, y): (R, y1)
            (R*(1-sin(pi/3)*sqrt(3)), R*sin(pi/3))
            (R*(1-sin(pi/3)*sqrt(3)), -R*sin(pi/3))
        */
        struct TriVert{
            sf::Vector2f v1, v2, v3;
        };

        TriVert triVert;
        triVert.v1 = {0.7f * radius, 0};

        triVert.v2 = {0.7f * radius * (1 - sinf(M_PI/3.0f) * sqrtf(3)),
        0.7f * radius * sinf(M_PI/3.0f)};

        triVert.v3 = {0.7f * radius * (1 - sinf(M_PI/3.0f) * sqrtf(3)),
        -0.7f * radius * sinf(M_PI/3.0f)};

        triangle.setPoint(0, triVert.v1);
        triangle.setPoint(1, triVert.v2);
        triangle.setPoint(2, triVert.v3);
        triangle.setFillColor(sf::Color::White);
        triangle.setPosition(xPos + radius , yPos + radius);

        buttonMemberData = {xPos, yPos, radius * 2, radius * 2};
        buttonMap[0] = buttonMemberData;
    }

    void createPause(){
        float xPos = (windowDimension_.x / 2) - (buttonDimension.x / 2);
        float yPos = (windowDimension_.y) - (buttonDimension.y * 5);

        // Circle part of the play button
        const float radius = buttonDimension.x;
        circle2.setRadius(radius);
        circle2.setOutlineThickness(radius * 0.25f);
        circle2.setOutlineColor(sf::Color::White);
        circle2.setFillColor(sf::Color::Black);
        circle2.setPosition(xPos, yPos);

        // Triangle part of the play button
        triangle2.setPointCount(3);

        // Triangle vertices
        /*
            The following formulas were used to determine the vertices
            of triangle inscribed in the circle shape:
            Law of Sines: a/sinA = b/sinB = c/sinC

            For any triangle inscribed in a circle
            2R = a/sinA where R is the radius

            For an isosceles triangle (we have a 45-45-90 triangle)
            B = C and b = c

            Using 45-45-90 we find the (x,y) diagonal point from the center of the circle
            assumming (0, 0) then the following coordinates can be used to draw two triangles
            (x, y): (R/sqrt(2), R/sqrt(2))
            (-R/sqrt(2), R/sqrt(2))
            (-R/sqrt(2), -R/sqrt(2))

            (-R/sqrt(2), R/sqrt(2))
            (R/sqrt(2), R/sqrt(2))
            (R/sqrt(2), -R/sqrt(2))
        */
        struct TriVert{
            sf::Vector2f v1, v2, v3;
        };
        
        TriVert triVert;
        triVert.v1 = {0.7f * radius/sqrtf(2), 0.7f * radius/sqrtf(2)};

        triVert.v2 = {-0.7f * radius/sqrtf(2), 0.7f * radius/sqrtf(2)};

        triVert.v3 = {-0.7f * radius/sqrtf(2), -0.7f * radius/sqrtf(2)};

        triangle2.setPoint(0, triVert.v1);
        triangle2.setPoint(1, triVert.v2);
        triangle2.setPoint(2, triVert.v3);
        triangle2.setFillColor(sf::Color::White);
        triangle2.setPosition(xPos + radius, yPos + radius);

        triangle3.setPointCount(3);

        TriVert triVert2;
        triVert2.v1 = {0.7f * radius/sqrtf(2), 0.7f * radius/sqrtf(2)};

        triVert2.v2 = {0.7f * radius/sqrtf(2), -0.7f * radius/sqrtf(2)};

        triVert2.v3 = {-0.7f * radius/sqrtf(2), -0.7f * radius/sqrtf(2)};

        triangle3.setPoint(0, triVert2.v1);
        triangle3.setPoint(1, triVert2.v2);
        triangle3.setPoint(2, triVert2.v3);
        triangle3.setFillColor(sf::Color::White);
        triangle3.setPosition(xPos + radius, yPos + radius);

        buttonMemberData = {xPos, yPos, radius * 2, radius * 2};
        buttonMap[1] = buttonMemberData;
    }

    void draw(sf::RenderTarget& target) const override{
        if (buttonType == 0){
            target.draw(circle);
            target.draw(triangle);
        }
        else if (buttonType == 1){
            target.draw(circle);
            target.draw(triangle2);
            target.draw(triangle3);
        }
    }

    void updateColor(sf::RenderWindow& window) override{
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        if (isHovered(mousePosition)){
            if(buttonType == 0){
                circle.setFillColor(sf::Color(210, 215, 211, 100));
            }
            else{
                circle2.setFillColor(sf::Color(210, 215, 211, 100));
                triangle2.setFillColor(sf::Color(210, 215, 211, 100));
                triangle3.setFillColor(sf::Color(210, 215, 211, 100));
            }
        }
        else{
            circle.setFillColor(sf::Color(sf::Color::Black)); 
            circle2.setFillColor(sf::Color(sf::Color::Black));
            triangle2.setFillColor(sf::Color(sf::Color::White));
            triangle3.setFillColor(sf::Color(sf::Color::White));
        }
    }

    
    void hoverText(sf::RenderWindow& window, sf::RenderTarget& target){
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        float xPos = (windowDimension_.x / 2) - (buttonDimension.x / 2);
        float yPos = (windowDimension_.y) - (buttonDimension.y * 7);

        if (isHovered(mousePosition)){
            if(buttonType == 0){
                description.setString("Play");
                description.setPosition(xPos, yPos);
                target.draw(description);
            }
            else{
                description.setString("Pause");
                description.setPosition(xPos, yPos);
                target.draw(description);
            }

        }
    }

    bool isClicked(const sf::RenderWindow& target) override{
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(target);

        for (const auto& button : buttonMap){
            addButtonMemberData(button.second);

            if (isHovered(mousePosition)) {
                if(buttonType == 0){
                    buttonType = 1;
                }
                else{
                    buttonType = 0;
                }               
                return true;
            }
        }
        return false;
    }

    ~PlayPause(){}
private:
    sf::Text description;
    sf::Font font_;
    sf::CircleShape circle, circle2;
    sf::ConvexShape triangle, triangle2, triangle3;
    sf::Vector2f buttonDimension = {30.0f, 30.0f};
    const sf::Vector2f windowDimension_;
    ButtonMemberData buttonMemberData;

    // Key 0 = play, 1 = pause
    std::unordered_map<int, ButtonMemberData> buttonMap;
};