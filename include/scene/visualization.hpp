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
        Visualization():timeOffset(sf::seconds(2.0f)), quit(4), seek(5){};

        void load(sf::RenderWindow& window) override {
            loadFont(font);

            audioProcessing = std::make_unique<AudioProcessing>(filePath_, message, quit, seek);

            windowDimension_.x = static_cast<float>(window.getSize().x);
            windowDimension_.y = static_cast<float>(window.getSize().y);
            musicPlayer = std::make_unique<MusicPlayer>(windowDimension_, font,
            audioProcessing -> getTotalMusicDuration());
            
            waveGeneration = std::make_unique<WaveGeneration>(message, quit, seek,
                audioProcessing -> getFrames(), font, windowDimension_);
        };

        void musicStatusRenderUpdate(){
            if (audioProcessing -> getMusicStatus() == sf::Music::Stopped){
                musicPlayer -> musicEndAction();
                audioProcessing -> updateCurrentMusicDuration(sf::Time::Zero);
                audioProcessing -> sendMusicStateUpdate(sf::Music::Stopped);
                /* waveGeneration -> setData(audioProcessing -> getMixDownBuffer(),
                    audioProcessing -> getMaxMag()); */

                waveGeneration -> sendMusicStateUpdate(sf::Music::Stopped,
                    audioProcessing -> getMixDownBuffer(), audioProcessing -> getMaxMag());
            }

            if (audioProcessing -> getMusicStatus() == sf::Music::Playing){ 
                audioProcessing -> updateCurrentMusicDuration(
                    (audioProcessing -> getCurrenMusicDuration())
                    + audioProcessing -> restartClock());
                audioProcessing -> sendMusicStateUpdate(sf::Music::Playing);
                /* waveGeneration -> setData(audioProcessing -> getMixDownBuffer(),
                    audioProcessing -> getMaxMag()); */

                waveGeneration -> sendMusicStateUpdate(sf::Music::Playing,
                    audioProcessing -> getMixDownBuffer(), audioProcessing -> getMaxMag());
            }
        }

        void render(sf::RenderWindow& window) override {
            musicPlayer -> render(window);
            musicStatusRenderUpdate();
            musicPlayer -> setMusicDuration(audioProcessing -> getCurrenMusicDuration());
            waveGeneration -> render(window);
        };

        void clickPause(){
            if (musicPlayer -> isMusicPaused()){
                audioProcessing -> pauseMusic();
                audioProcessing -> sendMusicStateUpdate(sf::Music::Paused);
                waveGeneration -> sendMusicStateUpdate(sf::Music::Paused,
                    audioProcessing -> getMixDownBuffer(), audioProcessing -> getMaxMag());
            }
            else{
                /* The reason we can't check against sf::Music::Pause is because the music could
                have stopped(after playing or in the beginning) or it is paused */ 
                if (audioProcessing -> getMusicStatus() != sf::Music::Playing){
                    audioProcessing -> playMusic();
                    
                    /*SFML setPlayingOffset: Changing the playing position when the stream is
                    stopped has no effect, since playing the stream would reset its position.
                    The codeblock tells SFML where to start playing the audio*/
                    audioProcessing -> setMusicPlayingOffset();
                    audioProcessing -> restartClock();
                    audioProcessing -> sendMusicStateUpdate(sf::Music::Playing);
                    waveGeneration -> sendMusicStateUpdate(sf::Music::Playing,
                        audioProcessing -> getMixDownBuffer(), audioProcessing -> getMaxMag());
                }
            }
        }

        void clickSkipForward(){
            if (musicPlayer -> isSkipForward()){
                if (((audioProcessing -> getCurrenMusicDuration()) + timeOffset)
                    <= audioProcessing -> getTotalMusicDuration()){
                    audioProcessing -> updateCurrentMusicDuration(
                        (audioProcessing -> getCurrenMusicDuration()) + timeOffset);
                    audioProcessing -> sendMusicStateUpdate(seek);
                    waveGeneration -> sendMusicStateUpdate(seek,
                        audioProcessing -> getMixDownBuffer(), audioProcessing -> getMaxMag());
                }
                else{
                    audioProcessing -> updateCurrentMusicDuration(
                        audioProcessing -> getTotalMusicDuration());
                }

                audioProcessing -> setMusicPlayingOffset();
                musicPlayer -> resetSkipForward();
            }
        }

        void clickSkipBackward(){
            if (musicPlayer -> isSkipBackward()){
                if (((audioProcessing -> getCurrenMusicDuration()) - timeOffset)
                    < sf::Time::Zero){

                    audioProcessing -> updateCurrentMusicDuration(sf::Time::Zero);
                    audioProcessing -> sendMusicStateUpdate(seek);
                    waveGeneration -> sendMusicStateUpdate(seek,
                        audioProcessing -> getMixDownBuffer(), audioProcessing -> getMaxMag());
                }
                else{
                    audioProcessing -> updateCurrentMusicDuration(
                        (audioProcessing -> getCurrenMusicDuration()) - timeOffset);
                }

                audioProcessing -> setMusicPlayingOffset();
                musicPlayer -> resetSkipBackward();
            }
        }

        void clickActions(sf::RenderWindow& window) override {
            musicPlayer -> clickActions(window);
            clickPause();
            clickSkipForward();
            clickSkipBackward();
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

        void setFilePath(const std::string& filePath) override{
            filePath_ = filePath;
        }
        
        void setVisual(const std::string& visual){
            typeOfVisual = visual;
            if(typeOfVisual == "WAVEFORM"){
                message = 0;
            }
            else if (typeOfVisual == "SPECTROGRAM"){
                message = 1;
            }
        }

        ~Visualization() = default;

    private:
        sf::Vector2f windowDimension_;
        sf::Font font;
        std::string filePath_, typeOfVisual;
        sf::Time timeOffset;
        uint16_t message, quit, seek;

        std::unique_ptr<AudioProcessing> audioProcessing;
        std::unique_ptr<MusicPlayer> musicPlayer;
        std::unique_ptr<WaveGeneration> waveGeneration;
};