#pragma once
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <complex>
#include "SFML/Audio/InputSoundFile.hpp"

class AudioProcessing {
public:
    
    AudioProcessing(const std::string& filePath_, const unsigned int sampleRate, const unsigned int channels)
    :filePath(filePath_), SAMPLE_RATE(sampleRate), CHANNELS(channels), channelAudioData(channels, std::vector<float>()){
        loadAudioFile();
        SAMPLE_COUNT = musicFile.getSampleCount();

        maxSample.resize(CHANNELS);
        for (unsigned int i = 0; i < CHANNELS; i++){
            maxSample[i] = 0.0f;
        }

        totalSampleCountPerChannel = SAMPLE_COUNT/CHANNELS;
    };

    void loadAudioFile() {
        if (!musicFile.openFromFile(filePath)){
            throw std::runtime_error("File Not Found");
        }
    }

    // Extract audio sample
    void extractAudioSample(){
        int counter = 0;
        sf::Int16* samples = new sf::Int16[CHANNELS];

        // Clears each channel in channel audio data
        for (std::vector<float>& channelData : channelAudioData) {channelData.clear();};

        while (counter < totalSampleCountPerChannel && musicFile.read(samples, CHANNELS)) {
            for (unsigned int i = 0; i < CHANNELS; ++i) {
                channelAudioData[i].push_back(samples[i]);
                
                if (maxSample[CHANNELS -1] != 0.0f){
                    if (samples[i] > maxSample[i]){
                        maxSample[i] = samples[i];
                    } 
                }

                else {
                    maxSample[i] = samples[i];
                }
            }
            counter++;
        }
        delete[] samples;
    }
    
    // Normalize audio Sample
    // amplitude ratio = (amp / max amp )* 0.80
    void normalizeSample(){
        for (unsigned int i = 0; i < totalSampleCountPerChannel; i++){
            for (unsigned int j = 0; i < CHANNELS; i++){
                channelAudioData[j][i] = (channelAudioData[j][i] / maxSample[j]) * 0.80;
            }
        }
    }
   
    ~AudioProcessing(){};

private:
    const std::string filePath;
    const unsigned int SAMPLE_RATE; //
    const unsigned int CHANNELS;
    unsigned int SAMPLE_COUNT;
    
    sf::InputSoundFile musicFile;
    std::vector<std::vector<float>> channelAudioData; // this is a vector of vectors of floats that stores audio data from different channels
    std::vector<float> maxSample; // stores the audio sample with the maximum amplitude
    int totalSampleCountPerChannel;
};