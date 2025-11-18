#pragma once
#include <vector>
#include <string>
#include "SFML/Audio/Music.hpp"
#include "scene/scene.hpp"
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

    void load(sf::RenderWindow& window) override{
        loadMusic();

        const unsigned int SAMPLE_RATE = music.getSampleRate();
        const unsigned int CHANNELS = music.getChannelCount();
        audioProcessing = new AudioProcessing(filePath_, SAMPLE_RATE, CHANNELS);
        //audioProcessing -> extractAudioSample();
        /*audioProcessing -> extractAudioSample();
        std::unordered_map<unsigned int, std::vector<float>> result = audioProcessing ->calculateSTFT();

        for (auto& pair:result){
            std::cout<<"Channel "<<pair.first <<"\n";
            for (int i =0; i<result[pair.first].size(); i++){
                std::cout<< result[pair.first][i]<<" ";
            }
            std::cout<<"\n";
        }
        */
        windowDimension_.x = static_cast<float>(window.getSize().x);
        windowDimension_.y = static_cast<float>(window.getSize().y);

        if(!font.loadFromFile("C:\\SFML-2.6.1\\Arial Font\\ARIAL.TTF")){
            throw std::runtime_error("Failed to load font");
        }

        musicPlayer = new MusicPlayer(windowDimension_, font);
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

        /* Wave Animation algorthm
            1. 2 float arrays for storing current and next waves rendered to the window
            2. Compute FFT for two windows, store earlier time samples in current and later in next array
            3. While song is playing, update the wave position and display to animate waves based on display time
            4. Before a wave reaches the edge of the window, deallocated the wave's memory and clear the array element
            5. Repeat step 4 until all the elements are cleared and wave memory deallocated in current array
            6. Reinitialize current array with next array, clear next array and compute FFT for the next window
            7. Reapeat steap 3-6 till the song ends

            Music State Functionality
            * Pause the wave animation if music stops and continue animation when music plays
            * Clear all animation on the window at the end of music and deallocate wave memory in current and nex array
        */

    };

    void clickActions(sf::RenderWindow& window) override{
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

    void cursorActions(sf::RenderWindow& window, sf::RenderTarget& target) override{
        musicPlayer -> cursorActions(window, target);
    }

    bool shouldMoveToNextScene() override{
        /* if(music.getStatus() == sf::Music::Stopped){
            return true;
        } */
        return false;
    }

    std::string getNextSceneId(std::vector<std::string>& sceneIds, std::string& sceneId) override{
        return "";
    }

    void setFilePath(const std::string& filePath) override{
        filePath_ = filePath;
    }

    void loadMusic(){
        if (!music.openFromFile(filePath_)){
            throw std::runtime_error("filePath not found");
        }
    }
    
    ~Visualization(){
        delete audioProcessing;
        delete musicPlayer;
        delete waveGeneration;
    }

private:
    sf::Vector2f windowDimension_;
    sf::Font font;
    std::string filePath_;
    sf::Music music;
    sf::Time currentMusicDuration = sf::Time::Zero;
    sf::Clock clock;

    std::vector<std::vector<float>> normalizedAudioSample; // stores audio samples(inner vector) in different channels (outer vector)
};