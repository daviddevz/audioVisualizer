#pragma once
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <complex>
//#include "SFML/Audio/InputSoundFile.hpp"
#include "SFML/Audio/SoundBuffer.hpp"

class AudioProcessing {
public:
    
    AudioProcessing(const std::string& filePath_, const unsigned int sampleRate, const unsigned int channels)
    :filePath(filePath_), SAMPLE_RATE(sampleRate), CHANNELS(channels), channelAudioData(channels, std::vector<sf::Int16>()){
        //loadAudioFile();
        SAMPLE_COUNT = buffer.getSampleCount();

     /*    maxSample.resize(CHANNELS);
        for (unsigned int i = 0; i < CHANNELS; i++){
            maxSample[i] = 0.0f;
        } */

        totalSamplePerChannel = SAMPLE_COUNT/CHANNELS;
    };

    void loadAudioFile() {
        if (!buffer.loadFromFile(filePath)){
            throw std::runtime_error("File Not Found");
        }
    }

    // Extract audio sample 16-bit PCM data -32,768 to 32,767
    void extractAudioSample(){
        const sf::Int16* samples  = buffer.getSamples(); // Pointer to all the audio samples

        // Clears each channel in channel audio data
        for (std::vector<sf::Int16>& channelData : channelAudioData) {channelData.clear();};

        // The read function is incorrect
        for(unsigned int i = 0; i < SAMPLE_COUNT; ++i){
            int channelIdx = i % CHANNELS; // ex. 0 or 1 for stereo
            channelAudioData[channelIdx].push_back(samples[i]);
        }

        /* // Test to print out 16-bit PCM data -32,768 to 32,767
        int counter = 1;
        for (std::vector<sf::Int16> chan : channelAudioData){
            std::cout<<"Channel "<< counter <<":";
            for(int i = 0; i < 4096; ++i){
                std::cout<<chan[i] <<" ";
            }
            std::cout<<std::endl;
            ++counter;
        } */
        delete[] samples;
    }
    
    // Refactor normalizeSample to extract the raw sample to find max
    // Normalize audio Sample
    // amplitude ratio = (amp / max amp )* 0.80
    /* std::vector<std::vector<float>> normalizeSample(){
        for (unsigned int i = 0; i < totalSamplePerChannel; i++){
            for (unsigned int j = 0; i < CHANNELS; i++){
                channelAudioData[j][i] = (channelAudioData[j][i] / maxSample[j]) * 0.80;
            }
        }
    } */
   
    ~AudioProcessing(){};

private:
    const std::string filePath;
    const unsigned int SAMPLE_RATE; //
    const unsigned int CHANNELS;
    unsigned int SAMPLE_COUNT;
    
    sf::SoundBuffer buffer;
    std::vector<std::vector<sf::Int16>> channelAudioData; // this is a vector of vectors of floats that stores audio data from different channels
    
    sf::Uint64 totalSamplePerChannel;
};