#pragma once

#include <thread>
#include <atomic>
#include <complex>
#include "buttons/upload.hpp"
#include "audio processing/stateUpdate.hpp"

using WaveList = std::vector<std::unique_ptr<UploadButton>>;
using AudioData = std::variant<std::vector<float>, std::vector<std::complex<double>>>;
constexpr float kMinMagnitude = 1e-12f;

class WaveGeneration{
    private:
        struct windowDim{
            unsigned int width, height;
        };

        struct buttonDim{
            float width, height;
        };

        windowDim winDim;
        buttonDim buttDim;
        buttonDim buttWindDim;
        float renderDisplacement, maxMag;
        sf::Font fnt;
        std::string text;
        int buttTxtFontSize;

        uint16_t typeOfVisual, QUIT, MUSIC_SEEK;
        uint64_t audioFrames;
        StateUpdate<uint16_t> messageStack;
        std::thread renderWaves;
        WaveList frontWaves; // For rendering
        WaveList backWaves; // For audio thread  writes
        std::vector<float> audioData;
        std::vector<std::complex<double>> audioDataComp;
        //AudioData audioData;

        std::mutex swapMutex;
        std::atomic<bool> hasNewWaves{false};

        void controlLoop(){
            const uint16_t MUSIC_STOPPED = sf::Music::Stopped;
            const uint16_t MUSIC_PAUSED = sf::Music::Paused;
            const uint16_t MUSIC_PLAYING = sf::Music::Playing;

            uint16_t message = 0, stopMsgCounter = 0;
            uint16_t& msgRef = message;
            std::vector<float>& audioDataRef = audioData;
            std::vector<std::complex<double>>& audioDataCompRef = audioDataComp;
            //AudioData& audioDataRef = audioData;
            float& maxMagRef = maxMag;

            while(true){
                if (typeOfVisual){messageStack.waitAndGet(msgRef, audioDataCompRef, maxMagRef);}
                else{messageStack.waitAndGet(msgRef, audioDataRef, maxMagRef);}

                if(message == MUSIC_STOPPED){
                    if (stopMsgCounter >= 1){ continue;}
                    updateWaves();
                    ++stopMsgCounter;
                }

                else if(message == MUSIC_PLAYING || message == MUSIC_SEEK){
                    updateWaves();
                }

                else if(message == MUSIC_PAUSED){
                    continue;
                }

                else if(message == QUIT){
                    break;
                }
            }
        }

    public:
        WaveGeneration(uint16_t visual, uint16_t quit, uint16_t seek, uint64_t frames, sf::Font ft,
            const sf::Vector2f windowDimension):typeOfVisual(visual), audioFrames(frames/2), fnt(ft){
            QUIT = quit;
            MUSIC_SEEK = seek;
            text = ""; // do not need text for waves
            buttTxtFontSize = 1;
            winDim = {static_cast<unsigned int>(windowDimension.x),
                static_cast<unsigned int>(windowDimension.y)};

            renderDisplacement = 0.1f;
            buttDim.width = (1 - renderDisplacement) * windowDimension.x / (audioFrames);
            buttDim.height = (1 - renderDisplacement) * windowDimension.y / 4.0f;
            buttWindDim = {windowDimension.x, windowDimension.y};
            renderWaves = std::thread(&WaveGeneration::controlLoop, this);
        };

        void render(sf::RenderWindow& window){
            // Swap if new data is ready
            if (hasNewWaves.load(std::memory_order_acquire)) {
                std::lock_guard<std::mutex> lock(swapMutex);
                frontWaves.swap(backWaves);
                hasNewWaves.store(false, std::memory_order_release);
            }

            for(const auto& wave : frontWaves){
                wave -> draw(window);
            }
        }

        /* std::vector<float> retrievBufferType(AudioData& pcmBuffer_){
            return std::get<std::vector<float>>(pcmBuffer_);
        }

        std::vector<std::complex<double>> retrievBufferCompType(AudioData& pcmBuffer_){
            return std::get<std::vector<std::complex<double>>>(pcmBuffer_);
        } */

        void sendMusicStateUpdate(uint16_t message, std::vector<float> pcmBuffer_, float maxMag_){
            messageStack.set(message, pcmBuffer_, maxMag_);
        }

        void sendMusicStateUpdate(uint16_t message, std::vector<std::complex<double>> pcmBuffer_,
            float maxMag_){
            messageStack.set(message, pcmBuffer_, maxMag_);
        }

        void sendMusicStateUpdate(uint16_t message, AudioData pcmBuffer_, float maxMag_){
            /* if (typeOfVisual){
                auto buffer = retrievBufferCompType(pcmBuffer_);
                messageStack.set(message, buffer, maxMag_);
            }
            else{
                auto buffer = retrievBufferType(pcmBuffer_);
                messageStack.set(message, buffer, maxMag_);
            } */

            std::visit([&](auto& buffer){messageStack.set(message, buffer, maxMag_);}, pcmBuffer_);
        }

        void updateWaves(){
            switch(typeOfVisual){
                case 1:
                {
                    if (audioDataComp.empty() || maxMag <= kMinMagnitude){ return;};

                    WaveList local;
                    local.reserve(audioDataComp.size());

                    int frameCounter = 0, xMove = -900, yMove = -50;

                    //waves.clear();
                    if (audioFrames != audioDataComp.size()){
                        audioFrames = audioDataComp.size();  
                        buttDim.width = (1 - renderDisplacement) * buttWindDim.width / (audioFrames);
                    }

                    for(std::complex<double> data : audioDataComp){
                        float safeMag = std::max(maxMag, kMinMagnitude); // safety to prevent division by 0
                        float mag = std::clamp((20 * std::log10(std::fabs(data) + kMinMagnitude)), -100.0, 0.0);
                        float tempHeight = mag / safeMag * buttDim.height;
                        auto wave = std::make_unique<UploadButton>(
                            sf::Vector2f(buttWindDim.width, buttWindDim.height),
                            fnt, text,
                            sf::Vector2f(buttDim.width, tempHeight),
                            buttTxtFontSize);
                        
                        wave -> moveButton(xMove + (buttDim.width * frameCounter), yMove);
                        
                        local.emplace_back(std::move(wave));
                        ++frameCounter;
                    }

                    // Swap phase
                    {
                        std::lock_guard<std::mutex> lock(swapMutex);
                        backWaves.swap(local);
                        hasNewWaves.store(true, std::memory_order_release);
                    }

                    break;
                }

                case 0:
                {
                    if (audioData.empty() || maxMag <= kMinMagnitude){ return;};

                    WaveList local;
                    local.reserve(audioData.size());

                    int frameCounter = 0, xMove = -900, yMove = -50;

                    //waves.clear();
                    if (audioFrames != audioData.size()){
                        audioFrames = audioData.size();  
                        buttDim.width = (1 - renderDisplacement) * buttWindDim.width / (audioFrames);
                    }

                    for(float data : audioData){
                        float safeMag = std::max(maxMag, kMinMagnitude); // safety to prevent division by 0
                        float tempHeight = (data/safeMag) * buttDim.height;

                        auto wave = std::make_unique<UploadButton>(
                            sf::Vector2f(buttWindDim.width, buttWindDim.height),
                            fnt, text,
                            sf::Vector2f(buttDim.width, tempHeight),
                            buttTxtFontSize);
                        
                        wave -> moveButton(xMove + (buttDim.width * frameCounter), yMove);

                        local.emplace_back(std::move(wave));
                        ++frameCounter;
                    }

                    // Swap phase
                    {
                        std::lock_guard<std::mutex> lock(swapMutex);
                        backWaves.swap(local);
                        hasNewWaves.store(true, std::memory_order_release);
                    }

                    break;
                }

                default:
                    break;
            }
        }

        ~WaveGeneration(){
            messageStack.set(QUIT);
            if(renderWaves.joinable()){
                renderWaves.join();
            }
        }
};



/* Introduction
    * This class will animate a rectangles in a moving window to represent PCM or frequency data
    * The size of the recetangle will be dynamically updated based on frames shown on the screen
    * Class constructor will take-in typeOfVisual as parameter
    * Class constructor will spin off a thread based music state model
    
Wave Animation algorthm
    1. While music is playing, display 512 frames or FFT to the window
    2. The thread responsibility is to update the size of wave for PCM data */
