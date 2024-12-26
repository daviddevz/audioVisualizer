#pragma once
#include "SFML/Window.hpp"
#include "buttons/playPause.hpp"
#include "buttons/skipForwardBackward.hpp"

class MusicPlayer{
private:
    //const std::string filePath_;
    bool playMusic = false, skipForward;

public:
    PlayPause* playPauseButton;
    SkipForwardBackward* skipForwardBackwardButton;
    
    MusicPlayer(const sf::Vector2f windowDimension, const sf::Font& font){
        playPauseButton = new PlayPause(windowDimension, font);
        skipForwardBackwardButton = new SkipForwardBackward(windowDimension, font);
    };

    void clickActions(sf::RenderWindow& window){
        //Initially the music is not played and play button is shown
        if (playPauseButton -> isClicked(window)){
            playMusic = !playMusic; //update playMusic to true if music is playing
        };

        if (skipForwardBackwardButton -> isClicked(window)){
            skipForward = true;
        }
        else{
            skipForward = false;
        }
    }

    void cursorActions(sf::RenderWindow& window, sf::RenderTarget& target){
        playPauseButton -> hoverText(window, target);
        playPauseButton -> updateColor(window);
        skipForwardBackwardButton -> hoverText(window, target);
        skipForwardBackwardButton -> updateColor(window);
    }

    void render(sf::RenderWindow& window){
        playPauseButton -> draw(window);
        skipForwardBackwardButton -> draw(window);
    }

    bool isMusicPaused(){
        if (playMusic == false){
            return true;
        }
        return false;
    }

    bool isSkipForward(){
        return skipForward;
    }

    void musicEndAction(){
        playPauseButton -> buttonType = 0;
        playMusic = false;
    }

    ~MusicPlayer(){
        delete playPauseButton;
        delete skipForwardBackwardButton;
    };
};