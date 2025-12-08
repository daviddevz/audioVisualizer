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
    //bool isPaused = false;

    //AudioProcessing* audioProcessing;
    std::unique_ptr<AudioProcessing> audioProcessing;
    MusicPlayer* musicPlayer;
    WaveGeneration* waveGeneration;
    
    Visualization() = default;

    void load(sf::RenderWindow& window) override {
        //loadMusic();
        loadFont(font);

        // Instantiate AudioProcessing
        audioProcessing = std::make_unique<AudioProcessing>(filePath_);

        // Instantiate MusicPlayer
        windowDimension_.x = static_cast<float>(window.getSize().x);
        windowDimension_.y = static_cast<float>(window.getSize().y);
        musicPlayer = new MusicPlayer(windowDimension_, font, audioProcessing -> getTotalMusicDuration());
    };

    void render(sf::RenderWindow& window) override {
        musicPlayer -> render(window);
        /* if (music.getStatus() == sf::Music::Stopped){
            musicPlayer -> musicEndAction();
            currentMusicDuration = sf::Time::Zero;
        } */

        if (audioProcessing -> getMusicStatus() == sf::Music::Stopped){
            musicPlayer -> musicEndAction();
            audioProcessing -> updateCurrentMusicDuration(sf::Time::Zero);
        }

        /* if (music.getStatus() == sf::Music::Playing && !isPaused){
            currentMusicDuration += clock.restart();
        } */

        if (audioProcessing -> getMusicStatus() == sf::Music::Playing
            && !(audioProcessing -> getIsPaused())){
            audioProcessing -> updateCurrentMusicDuration((audioProcessing -> getCurrenMusicDuration())
                                                        + audioProcessing -> restartClock());
        }

        //musicPlayer -> getMusicDuration(currentMusicDuration);
        musicPlayer -> getMusicDuration(audioProcessing -> getCurrenMusicDuration());
    };

    void clickActions(sf::RenderWindow& window) override {
        musicPlayer -> clickActions(window);
        
        if (musicPlayer -> isMusicPaused() == false){
            /* if (music.getStatus() != sf::Music::Playing){
                music.play();
                
                if (isPaused) {
                    music.setPlayingOffset(currentMusicDuration); 
                }
                clock.restart();
                isPaused = false;
            } */

            if (audioProcessing -> getMusicStatus() != sf::Music::Playing){
                //music.play();
                audioProcessing -> playMusic();
                
                if (audioProcessing -> getIsPaused()) {
                    audioProcessing -> setMusicPlayingOffset(); 
                }
                //clock.restart();
                //isPaused = false;
                audioProcessing -> restartClock();
                audioProcessing -> setIsPaused(false);
            }
        }
        else{
            //music.pause();
            //isPaused = true;

            audioProcessing -> pauseMusic();
            audioProcessing -> setIsPaused(true);
        }

        if (musicPlayer -> isSkipForward()){
            /* if (music.getStatus() != sf::Music::Playing || music.getStatus() != sf::Music::Paused){
                //sf::Time timeOffset = sf::seconds(2.0f);

                if ((currentMusicDuration + timeOffset) > music.getDuration()){
                    currentMusicDuration = music.getDuration();
                }
                else{
                    currentMusicDuration += timeOffset;
                }
                music.setPlayingOffset(currentMusicDuration); 
                musicPlayer -> resetSkipForward();
            } */

            if (audioProcessing -> getMusicStatus() != sf::Music::Playing
            || audioProcessing -> getMusicStatus() != sf::Music::Paused){
                if (((audioProcessing -> getCurrenMusicDuration()) + timeOffset)
                > audioProcessing -> getTotalMusicDuration()){
                    //currentMusicDuration = music.getDuration();
                    audioProcessing -> updateCurrentMusicDuration(audioProcessing -> getTotalMusicDuration());
                }
                else{
                    //currentMusicDuration += timeOffset;
                    audioProcessing -> updateCurrentMusicDuration((audioProcessing -> getCurrenMusicDuration()) + timeOffset);
                }
                //music.setPlayingOffset(currentMusicDuration); 
                audioProcessing -> setMusicPlayingOffset();
                musicPlayer -> resetSkipForward();
            }
        }
        
        if (musicPlayer -> isSkipBackward() == true){
            /* if (music.getStatus() != sf::Music::Playing || music.getStatus() != sf::Music::Paused){
                //sf::Time timeOffset = sf::seconds(2.0f);

                if ((currentMusicDuration - timeOffset) < sf::Time::Zero){
                    currentMusicDuration = sf::Time::Zero;
                }
                else{
                    currentMusicDuration -= timeOffset;
                }
                music.setPlayingOffset(currentMusicDuration);
                musicPlayer -> resetSkipBackward();
            } */

            if (audioProcessing -> getMusicStatus() != sf::Music::Playing
            || audioProcessing -> getMusicStatus() != sf::Music::Paused){
                if (((audioProcessing -> getCurrenMusicDuration()) - timeOffset) < sf::Time::Zero){
                    //currentMusicDuration = sf::Time::Zero;
                    audioProcessing -> updateCurrentMusicDuration(sf::Time::Zero);
                }
                else{
                    //currentMusicDuration -= timeOffset;
                    audioProcessing -> updateCurrentMusicDuration((audioProcessing -> getCurrenMusicDuration()) - timeOffset);
                }
                //music.setPlayingOffset(currentMusicDuration);
                audioProcessing -> setMusicPlayingOffset();
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

    /* void loadMusic() {
        if (!music.openFromFile(filePath_)){
            throw std::runtime_error("filePath not found");
        }
    } */
    
    ~Visualization() {
        delete musicPlayer;
    };

private:
    sf::Vector2f windowDimension_;
    sf::Font font;
    std::string filePath_;
    //sf::Music music;
    //sf::Time currentMusicDuration = sf::Time::Zero;
    //sf::Clock clock;
    sf::Time timeOffset = sf::seconds(2.0f);

    std::vector<std::vector<float>> normalizedAudioSample; // stores audio samples(inner vector) in different channels (outer vector)
};