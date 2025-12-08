#pragma once
#include <vector>
#include <string>
#include "scene/scene.hpp"
#include "SFML/Audio/Music.hpp"
#include "audio processing/audioProcessing.hpp"
#include "visualization/musicPlayer.hpp"
#include "visualization/waveGeneration.hpp"

class Visualization : public Scene {
public:
    bool isPaused = false;

    AudioProcessing* audioProcessing;
    MusicPlayer* musicPlayer;
    WaveGeneration* waveGeneration;
    
    Visualization() = default;

    void load(sf::RenderWindow& window) override {
        loadMusic();
        loadFont(font);

        // Instantiate AudioProcessing
        std::unique_ptr<AudioProcessing> audioProcessing = std::make_unique<AudioProcessing>(filePath_);

        // Instantiate MusicPlayer
        windowDimension_.x = static_cast<float>(window.getSize().x);
        windowDimension_.y = static_cast<float>(window.getSize().y);
        musicPlayer = new MusicPlayer(windowDimension_, font, audioProcessing -> getTotalMusicDuration());
    };

    void render(sf::RenderWindow& window) override {
        musicPlayer -> render(window);

        if (music.getStatus() == sf::Music::Stopped){
            musicPlayer -> musicEndAction();
            currentMusicDuration = sf::Time::Zero;
        }

        /* if (audioProcessing -> getStatus() == sf::Music::Stopped){
            musicPlayer -> musicEndAction();
            audioProcessing -> updateCurrentMusicDuration(sf::Time::Zero);
        } */

        if (music.getStatus() == sf::Music::Playing && !isPaused){
            currentMusicDuration += clock.restart();
        }

        /* if (audioProcessing -> getStatus() == sf::Music::Playing && !(audioProcessing -> getIsPaused())){
            audioProcessing -> updateCurrentMusicDuration((audioProcessing -> getCurrenMusicDuration())
                                                        + clock.restart());
        } */

        musicPlayer -> getMusicDuration(currentMusicDuration, music.getDuration());
    };

    void clickActions(sf::RenderWindow& window) override {
        musicPlayer -> clickActions(window);
        
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
                sf::Time timeOffset = sf::seconds(2.0f);

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
                sf::Time timeOffset = sf::seconds(2.0f);

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

    void cursorActions(sf::RenderWindow& window, sf::RenderTarget& target) override {
        musicPlayer -> cursorActions(window, target);
    }

    bool shouldMoveToNextScene() override {
        /* if(music.getStatus() == sf::Music::Stopped){
            return true;
        } */
        return false;
    }

    /* std::string getNextSceneId(std::vector<std::string>& sceneIds, std::string& sceneId) override{
        return "";
    } */

    void setFilePath(const std::string& filePath) override {
        filePath_ = filePath;
    }

    void loadMusic() {
        if (!music.openFromFile(filePath_)){
            throw std::runtime_error("filePath not found");
        }
    }
    
    ~Visualization() {
        delete musicPlayer;
    };

private:
    sf::Vector2f windowDimension_;
    sf::Font font;
    std::string filePath_;
    sf::Music music;
    sf::Time currentMusicDuration = sf::Time::Zero;
    sf::Clock clock;

    std::vector<std::vector<float>> normalizedAudioSample; // stores audio samples(inner vector) in different channels (outer vector)
};