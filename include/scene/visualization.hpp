#pragma once

#include <vector>
#include <string>
#include "SFML/Audio/Music.hpp"
#include "scene/scene.hpp"
#include "audio processing/audioProcessing.hpp"
#include "visualization/musicPlayer.hpp"
#include "visualization/waveAnimation.hpp"

class Visualization : public Scene {
public:
    bool isPaused = false;
    MusicPlayer* musicPlayer;
    WaveAnimation* waveAnimation;
    Visualization(){};

    void load(sf::RenderWindow& window) override{
        windowDimension_.x = static_cast<float>(window.getSize().x);
        windowDimension_.y = static_cast<float>(window.getSize().y);

        if(!font.loadFromFile("C:\\SFML-2.6.1\\Arial Font\\ARIAL.TTF")){
            throw std::runtime_error("Failed to load font");
        }

        musicPlayer = new MusicPlayer(windowDimension_, font);
        //waveAnimation = new WaveAnimation();
    };

    void render(sf::RenderWindow& window) override{
        musicPlayer -> render(window);

        if (music.getStatus() == sf::Music::Stopped){
            musicPlayer -> musicEndAction();
            currentMusicDuration = sf::Time::Zero;
        }

        if (music.getStatus() == sf::Music::Playing && !isPaused){
            currentMusicDuration += clock.restart();
        }
        
        musicPlayer -> getMusicDuration(currentMusicDuration, music.getDuration());
    };

    void clickActions(sf::RenderWindow& window) override{
        musicPlayer -> clickActions(window);
        //waveAnimation -> clicActions(window, musicPlayer -> isMusicPaused());
        
        if (musicPlayer -> isMusicPaused() == false){
            if (music.getStatus() != sf::Music::Playing){
                music.play();
                
                if (isPaused) {
                    music.setPlayingOffset(currentMusicDuration); 
                }
                clock.restart();
                isPaused = false;
            }
        }
        else{
            music.pause();
            isPaused = true;
            
        }

        if (musicPlayer -> isSkipForward()){
            if (music.getStatus() != sf::Music::Playing || music.getStatus() != sf::Music::Paused){
                sf::Time timeOffset = sf::seconds(30.0f);

                if ((currentMusicDuration + timeOffset) > music.getDuration()){
                    currentMusicDuration = music.getDuration();
                }
                else{
                    currentMusicDuration += timeOffset;
                }
                music.setPlayingOffset(currentMusicDuration); 
                musicPlayer -> resetSkipForward();
            }
        }
        
        if (musicPlayer -> isSkipBackward() == true){
            if (music.getStatus() != sf::Music::Playing || music.getStatus() != sf::Music::Paused){
                sf::Time timeOffset = sf::seconds(10.0f);

                if ((currentMusicDuration - timeOffset) < sf::Time::Zero){
                    currentMusicDuration = sf::Time::Zero;
                }
                else{
                    currentMusicDuration -= timeOffset;
                }
                music.setPlayingOffset(currentMusicDuration);
                musicPlayer -> resetSkipBackward();
            }
        }
    }

    void cursorActions(sf::RenderWindow& window, sf::RenderTarget& target) override{
        musicPlayer -> cursorActions(window, target);
    }

    bool shouldMoveToNextScene() override{
        /* if(music.getStatus() == sf::Music::Stopped){
            return true;
        } */
        return false;
    }

    std::string getNextSceneId() override{
        return "";
    }

    void setFilePath(const std::string& filePath) override{
        filePath_ = filePath;
        loadMusic();
    }
    
    void setSTFTData (const std::vector<std::vector<std::complex<double>>>& stftData){
        
    }

    void loadMusic(){
        if (!music.openFromFile(filePath_)){
            throw std::runtime_error("filePath not found");
        }
    }

    ~Visualization(){
        delete musicPlayer;
        delete waveAnimation;
    }

private:
    sf::Vector2f windowDimension_;
    sf::Font font;
    std::string filePath_;
    sf::Music music;
    sf::Time currentMusicDuration = sf::Time::Zero;
    sf::Clock clock;
    const std::vector<std::vector<std::complex<double>>> stftData_; 
    const int videoFps = 30; 
    // ... other private members (e.g., for storing visualization data, rendering)

    void downsampleSTFT(); 
    void renderSpectrum(sf::RenderWindow& window, const std::vector<std::complex<double>>& spectrum); 
    // ... other private helper functions
};