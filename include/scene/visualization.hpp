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
        std::unique_ptr<AudioProcessing> audioProcessing;
        MusicPlayer* musicPlayer;
        WaveGeneration* waveGeneration;
        
        Visualization() = default;

        void load(sf::RenderWindow& window) override {
            loadFont(font);

            audioProcessing = std::make_unique<AudioProcessing>(filePath_);

            windowDimension_.x = static_cast<float>(window.getSize().x);
            windowDimension_.y = static_cast<float>(window.getSize().y);
            musicPlayer = new MusicPlayer(windowDimension_, font,
            audioProcessing -> getTotalMusicDuration());
        };

        void musicStatusRenderUpdate(){
            if (audioProcessing -> getMusicStatus() == sf::Music::Stopped){
                musicPlayer -> musicEndAction();
                audioProcessing -> updateCurrentMusicDuration(sf::Time::Zero);
            }

            if (audioProcessing -> getMusicStatus() == sf::Music::Playing
                && !(audioProcessing -> getIsPaused())){

                audioProcessing -> updateCurrentMusicDuration(
                    (audioProcessing -> getCurrenMusicDuration())
                    + audioProcessing -> restartClock());
            }
        }

        void render(sf::RenderWindow& window) override {
            musicPlayer -> render(window);
            musicStatusRenderUpdate();
            musicPlayer -> getMusicDuration(audioProcessing -> getCurrenMusicDuration());
        };

        // TO-DO: abstract out the if statatements in a function
        void clickActions(sf::RenderWindow& window) override {
            musicPlayer -> clickActions(window);
            
            if (musicPlayer -> isMusicPaused() == false){
                if (audioProcessing -> getMusicStatus() != sf::Music::Playing){
                    audioProcessing -> playMusic();
                    
                    if (audioProcessing -> getIsPaused()) {
                        audioProcessing -> setMusicPlayingOffset(); 
                    }

                    audioProcessing -> restartClock();
                    audioProcessing -> setIsPaused(false);
                }
            }
            else{
                audioProcessing -> pauseMusic();
                audioProcessing -> setIsPaused(true);
            }

            if (musicPlayer -> isSkipForward()){
                if (audioProcessing -> getMusicStatus() != sf::Music::Playing
                    || audioProcessing -> getMusicStatus() != sf::Music::Paused){
                    
                    if (((audioProcessing -> getCurrenMusicDuration()) + timeOffset)
                        > audioProcessing -> getTotalMusicDuration()){

                        audioProcessing -> updateCurrentMusicDuration(
                            audioProcessing -> getTotalMusicDuration());
                    }
                    else{
                        audioProcessing -> updateCurrentMusicDuration(
                            (audioProcessing -> getCurrenMusicDuration()) + timeOffset);
                    }

                    audioProcessing -> setMusicPlayingOffset();
                    musicPlayer -> resetSkipForward();
                }
            }
            
            if (musicPlayer -> isSkipBackward() == true){

                if (audioProcessing -> getMusicStatus() != sf::Music::Playing
                    || audioProcessing -> getMusicStatus() != sf::Music::Paused){

                    if (((audioProcessing -> getCurrenMusicDuration()) - timeOffset)
                        < sf::Time::Zero){

                        audioProcessing -> updateCurrentMusicDuration(sf::Time::Zero);
                    }
                    else{
                        audioProcessing -> updateCurrentMusicDuration(
                            (audioProcessing -> getCurrenMusicDuration()) - timeOffset);
                    }

                    audioProcessing -> setMusicPlayingOffset();
                    musicPlayer -> resetSkipBackward();
                }
            }
        }

        void cursorActions(sf::RenderWindow& window, sf::RenderTarget& target) override {
            musicPlayer -> cursorActions(window, target);
        }

        bool shouldMoveToNextScene() override {
            return false;
        }

        /* std::string getNextSceneId(std::vector<std::string>& sceneIds, std::string& sceneId) override{
            return "";
        } */

        void setFilePath(const std::string& filePath) override {
            filePath_ = filePath;
        }
        
        ~Visualization() {
            delete musicPlayer;
        };

    private:
        sf::Vector2f windowDimension_;
        sf::Font font;
        std::string filePath_;
        sf::Time timeOffset = sf::seconds(2.0f);

        // stores audio samples(inner vector) in different channels (outer vector)
        std::vector<std::vector<float>> normalizedAudioSample; 
};