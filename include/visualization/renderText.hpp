#pragma once
#include <iostream>
#include <string>
#include "SFML/Graphics.hpp"

class RenderText{
public:
    RenderText(){};
    RenderText(const sf::Font& font, const std::string& str, sf::Color& fillColor,
    const sf::Vector2f& position, unsigned int characterSize){
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(characterSize);
        text.setFillColor(fillColor);
        text.setPosition(position);
        text.setStyle(sf::Text::Bold);
    };

    void draw(sf::RenderWindow& target) const {
        target.draw(text);
    }

    void setStr(std::string& str){
        text.setString(str);
    }

    void setPosition(sf::Vector2f& position){
        text.setPosition(position);
    }
    
private:
    sf::Text text;
};