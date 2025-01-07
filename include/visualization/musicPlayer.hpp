#pragma once
#include "SFML/Window.hpp"
#include "buttons/playPause.hpp"
#include "buttons/skipForward.hpp"
#include "buttons/skipBackward.hpp"
#include "musicDurationBar.hpp"

class MusicPlayer{
private:
    //const std::string filePath_;
    bool playMusic = false, skipForward = false, skipBackward = false;

public:
    PlayPause* playPauseButton;
    SkipForward* skipForwardButton;
    SkipBackward* skipBackwardButton;
    MusicDuration* musicDurationBar;
    
    MusicPlayer(const sf::Vector2f windowDimension, const sf::Font& font){
        playPauseButton = new PlayPause(windowDimension, font);
        skipForwardButton = new SkipForward(windowDimension, font);
        skipBackwardButton = new SkipBackward(windowDimension, font);
        musicDurationBar = new MusicDuration(windowDimension, font);
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
        playPauseButton -> updateColor(window);
        skipForwardButton -> updateColor(window);
        skipBackwardButton -> updateColor(window);
    }

    void render(sf::RenderWindow& window){
        playPauseButton -> draw(window);
        skipForwardButton -> draw(window);
        skipBackwardButton -> draw(window);
        musicDurationBar -> draw(window);
        musicDurationBar -> renderTimer(window);
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

    void getMusicDuration(sf::Time& currDuration, const sf::Time& totalDuration){
        musicDurationBar -> setMusicDuration(currDuration, totalDuration);
    }

    ~MusicPlayer(){
        delete playPauseButton;
        delete skipForwardButton;
        delete skipBackwardButton;
        delete musicDurationBar;
    };
};