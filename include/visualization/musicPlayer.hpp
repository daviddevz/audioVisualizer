#pragma once
#include "SFML/Window.hpp"
#include "buttons/playPause.hpp"
#include "buttons/skipForward.hpp"
#include "buttons/skipBackward.hpp"

class MusicPlayer{
private:
    //const std::string filePath_;
    bool playMusic = false, skipForward = false, skipBackward = false;

public:
    PlayPause* playPauseButton;
    SkipForward* skipForwardButton;
    SkipBackward* skipBackwardButton;
    
    MusicPlayer(const sf::Vector2f windowDimension, const sf::Font& font){
        playPauseButton = new PlayPause(windowDimension, font);
        skipForwardButton = new SkipForward(windowDimension, font);
        skipBackwardButton = new SkipBackward(windowDimension, font);
    };

    void clickActions(sf::RenderWindow& window){
        //Initially the music is not played and play button is shown
        if (playPauseButton -> isClicked(window)){
            playMusic = !playMusic;
        }

        if (skipForwardButton -> isClicked(window) == true){
            skipForward = true;
        }
        
        if (skipBackwardButton -> isClicked(window) == true){
            skipBackward = true;
        }
    }

    void cursorActions(sf::RenderWindow& window, sf::RenderTarget& target){
        playPauseButton -> hoverText(window, target);
        playPauseButton -> updateColor(window);
        skipForwardButton -> hoverText(window, target);
        skipForwardButton -> updateColor(window);
        skipBackwardButton -> hoverText(window, target);
        skipBackwardButton -> updateColor(window);
    }

    void render(sf::RenderWindow& window){
        playPauseButton -> draw(window);
        skipForwardButton -> draw(window);
        skipBackwardButton -> draw(window);
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

    void resetSkipForward(){
        skipForward = false;
    }
    
    bool isSkipBackward(){
        return skipBackward;
    }

    void resetSkipBackward(){
        skipBackward = false;
    }

    void musicEndAction(){
        playPauseButton -> buttonType = 0;
        playMusic = false;
    }

    ~MusicPlayer(){
        delete playPauseButton;
        delete skipForwardButton;
        delete skipBackwardButton;
    };
};