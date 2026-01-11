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
        uint32_t sampleFreq;
        StateUpdate<uint16_t> messageStack;
        std::thread renderWaves;
        WaveList frontWaves; // For rendering
        WaveList backWaves; // For audio thread  writes
        std::vector<float> audioData;
        std::vector<std::complex<double>> audioDataComp;

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
            float& maxMagRef = maxMag;

            while(true){
                if (typeOfVisual){messageStack.waitAndGet(msgRef, audioDataCompRef, maxMagRef);}
                else{messageStack.waitAndGet(msgRef, audioDataRef, maxMagRef);}

                if(message == MUSIC_STOPPED){
                    // Don't create any waves
                    if (typeOfVisual){
                        auto zeros = -std::complex<double>(kMinMagnitude, kMinMagnitude);
                        std::fill(audioDataComp.begin(), audioDataComp.end(), zeros);
                    }
                    else{
                        std::fill(audioData.begin(), audioData.end(), 0.0f);
                    }

                    updateWaves();
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
        WaveGeneration(uint16_t visual, uint16_t quit, uint16_t seek, sf::Font ft, uint64_t frames,
            const sf::Vector2f windowDimension, uint32_t sampRate):typeOfVisual(visual),
            audioFrames(frames/2), fnt(ft), sampleFreq(sampRate){
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

        void sendMusicStateUpdate(uint16_t message, std::vector<float> pcmBuffer_, float maxMag_){
            messageStack.set(message, pcmBuffer_, maxMag_);
        }

        void sendMusicStateUpdate(uint16_t message, std::vector<std::complex<double>> pcmBuffer_,
            float maxMag_){
            messageStack.set(message, pcmBuffer_, maxMag_);
        }

        void sendMusicStateUpdate(uint16_t message, AudioData pcmBuffer_, float maxMag_){
            std::visit([&](auto& buffer){messageStack.set(message, buffer, maxMag_);}, pcmBuffer_);
        }

        std::vector<double> getLogFreq(size_t sampleSize){
            int groups = 16; // Frequency groups
            double freqMin = static_cast<double>(sampleFreq) / sampleSize;
            double freqMax = static_cast<double>(sampleFreq) / 2; // Nyquist Sampling Frequency Theorem

            double logMin = std::log10(freqMin);
            double logMax = std::log10(freqMax);
            double logSlope = (logMax - logMin) / groups;

            std::vector<double> freqBins(groups + 1); //+1 space for min and max frequencies

            for (int i = 0; i <= groups; ++i){
                double exponent = logMin + (static_cast<double>(i) * logSlope);
                freqBins[i] = std::pow(10.0, exponent);
            }

            return freqBins;
        }

        void averageFreq (std::vector<std::complex<double>>& samples, std::vector<double>& freqBins){
            int N = samples.size() / 2; 
            double binFactor = N / static_cast<double>(sampleFreq);

            // Stores the average mangitude of logarithm scale frequency bins
            int groups = freqBins.size() - 1;
            std::vector<double> avgMagOfLogFreqBins(groups);

            int lastSampleIdx = 1;

            for(int i = 0; i < groups; ++i){
                int binIdx = std::round(binFactor * freqBins[i + 1]);
                double magSum = 0;
                int count = 0;
                
                for(int j = lastSampleIdx; j < binIdx; ++j){
                    magSum += std::fabs(samples[j]); // complex number 
                    ++count;
                }
                if (count > 0) {
                    avgMagOfLogFreqBins[i] = magSum / count;
                } else {
                    avgMagOfLogFreqBins[i] = 0;
                }

                lastSampleIdx = binIdx;
            }

            freqBins = avgMagOfLogFreqBins;
        }

        void updateWaves(){
            switch(typeOfVisual){
                case 1:
                {
                    if (audioDataComp.empty() || maxMag <= kMinMagnitude){ return;};

                    WaveList local;
                    size_t sampleSize = audioDataComp.size();
                    local.reserve(sampleSize);

                    int frameCounter = 0, xMove = -350, yMove = -50;

                    std::vector<double> freqBins = getLogFreq(sampleSize);
                    averageFreq(audioDataComp, freqBins);

                    sampleSize = freqBins.size() * 3; // Update sample size

                    if (audioFrames != sampleSize){
                        audioFrames = sampleSize;  
                        buttDim.width = (1 - renderDisplacement) * buttWindDim.width / (audioFrames);
                    }

                    buttDim.height = (1 - renderDisplacement) * buttWindDim.height / 1.50f;

                    for(double data : freqBins){
                        float safeMag = maxMag;
                        float mag = std::clamp((20 * std::log10(std::fabs(data) + kMinMagnitude)),
                            -static_cast<double>(maxMag), 0.0); // decibels formula (dB)

                        float tempHeight = (mag + safeMag) / safeMag * buttDim.height;
                        auto wave = std::make_unique<UploadButton>(
                            sf::Vector2f(buttWindDim.width, buttWindDim.height),
                            fnt, text,
                            sf::Vector2f(buttDim.width, tempHeight),
                            buttTxtFontSize);
                        
                        wave -> moveButton(xMove + (buttDim.width * frameCounter)*1.1, yMove);
                        local.emplace_back(std::move(wave));

                        ++frameCounter;
                        if (frameCounter > (sampleSize/2 + 1)){break;}
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
