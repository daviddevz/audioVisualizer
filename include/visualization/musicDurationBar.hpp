#pragma once
#include "progressBar.hpp"
#include "renderText.hpp"
#include <cmath>

class MusicDuration : public ProgressBar{
public:
    static const sf::Vector2f shapeDimension; // Declare as static const
    static const sf::Color backgroundColor;
    static const sf::Color maskedColor;

    MusicDuration(sf::Vector2f windowDimension, const sf::Font& font_, const sf::Time& totalDuration_): 
    ProgressBar({windowDimension.x * (windowDimension.x - shapeDimension.x) / (2.0f * windowDimension.x),
    windowDimension.y * 0.80f}, backgroundColor, maskedColor, shapeDimension), font(font_),
    totalDuration(totalDuration_){};
    

    std::string secondToHHMMSS(sf::Time& time){
        int hour = (time.asSeconds() >= 3600.00f) ? time.asSeconds()/3600 : 0;
        int minutes = (fmod(time.asSeconds(), 3600.0f) >= 60.0f) ? fmod(time.asSeconds(), 3600)/60 : 0;
        int seconds = (time.asSeconds() - (hour * 3600 + minutes * 60));
        
        std::string HHMMSS;

        if ((hour / 10) >= 1){
            HHMMSS = std::to_string(hour) + ":";
        }
        else{
            HHMMSS = "0" + std::to_string(hour) + ":";
        }

        if ((minutes / 10) >= 1){
            HHMMSS += std::to_string(minutes) + ":";
        }
        else{
            HHMMSS += "0" + std::to_string(minutes) + ":";
        }

        if ((seconds / 10) >= 1){
            HHMMSS += std::to_string(seconds);
        }
        else{
            HHMMSS += "0" + std::to_string(seconds);
        }

        return HHMMSS;
    }

    //void setMusicDuration (sf::Time& currDuration_, const sf::Time& totalDuration_)
    void setMusicDuration (sf::Time& currDuration_){
        currDuration = currDuration_; 
        //totalDuration = totalDuration_;

        updateProgressBar(currDuration.asSeconds() / totalDuration.asSeconds());
    }

    void renderTimer(sf::RenderWindow& target){
        RenderText currDurTxt, remainingDuration;

        std::string currDur = secondToHHMMSS(currDuration);

        sf::Time remainingTime = sf::seconds(totalDuration.asSeconds() - currDuration.asSeconds());
        std::string totalDur = secondToHHMMSS(remainingTime);

        sf::Color fillColor = sf::Color::White;
        sf::Vector2f currDurPos = getShapePos();
        currDurPos.x -= SHIFT * 1.5f;
        currDurPos.y -= shapeDimension.y * 2.0f;
        unsigned int charSize = 30;
        
        currDurTxt = RenderText(font, currDur, fillColor, currDurPos, charSize);

        sf::Vector2f remaingDurPos = getShapePos();
        remaingDurPos.x += getShapeDim().x + SHIFT * 0.25f;
        remaingDurPos.y -= shapeDimension.y * 1.5f;

        remainingDuration = currDurTxt;
        remainingDuration.setStr(totalDur);
        remainingDuration.setPosition(remaingDurPos);

        currDurTxt.draw(target);
        remainingDuration.draw(target);
    }

    ~MusicDuration() = default;
private:
    const float SHIFT = 100.00f;
    sf::Font font;
    sf::Time currDuration, totalDuration;
};