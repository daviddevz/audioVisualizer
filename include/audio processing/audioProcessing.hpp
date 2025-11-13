#pragma once
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <complex>
//#include "FFT.hpp"
#include "SFML/Audio/InputSoundFile.hpp"

class AudioProcessing {
public:
    //std::vector<FFT*> STFT;
    
    AudioProcessing(const std::string& filePath_, const unsigned int sampleRate, const unsigned int channels)
    :filePath(filePath_), SAMPLE_RATE(sampleRate), CHANNELS(channels), channelAudioData(channels, std::vector<float>()){
        loadAudioFile();
        SAMPLE_COUNT = musicFile.getSampleCount();
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

        int totalSampleCountPerChannel = SAMPLE_COUNT/CHANNELS;

        while (counter < totalSampleCountPerChannel && musicFile.read(samples, CHANNELS)) {
            for (unsigned int i = 0; i < CHANNELS; ++i) {
                channelAudioData[i].push_back(samples[i]);
            }
            counter++;
        }
        delete[] samples;
    }
    
    /*// STFT - Short Time Fourier Transform
    // The size of the hashMap will be equal to FFT_SIZE
    // The size of the vector will be (total audio sample / FFT_SIZE) rounded to nearest integer
    std::unordered_map<unsigned int, std::vector<float>> calculateSTFT(){
        unsigned int channelCounter = 0;
        std::unordered_map<unsigned int, std::vector<float>> FFT_Result(CHANNELS);
        
        for (std::vector<float>& channelData : channelAudioData) {
            channelCounter += 1;
            STFT.push_back(new FFT(channelData, FFT_SIZE/CHANNELS));
            std::vector<float> result = STFT[channelCounter - 1] -> calculate_dft();
            FFT_Result[channelCounter] = normalize(result);
        }

        for (unsigned int i = 0; i< CHANNELS; i++){
            delete STFT[i];
        }
        STFT.clear();

        return FFT_Result;
    };*/

    /*std::vector<float>normalize (std::vector<float>& data){
        float maxValue = *std::max_element(data.begin(), data.end());
        std::cout<<"max value "<<maxValue<<"\n";

        for (int i =0; i < data.size(); i++){
            data[i] /= maxValue;
        }
        return data;
    }*/

    /*unsigned int getFFTSampleSize(){
        return (FFT_SIZE / CHANNELS) / 2; // Nyquist Theorem for sampling frequencies
    }*/

    ~AudioProcessing(){
        /*for (unsigned int i = 0; i< CHANNELS; i++){
            //delete STFT[i];
        }*/
        
    };

private:
    const std::string filePath;
    const unsigned int SAMPLE_RATE; //Hz
    const unsigned int CHANNELS;
    //const unsigned int FFT_SIZE = 4096; // 1 << 12 = 4096 audio samples considered per FFT. Alway a power of 2
    //const float BIN_WIDTH; //Hz space between frequency bins
    unsigned int SAMPLE_COUNT;
    
    sf::InputSoundFile musicFile;
    std::vector<std::vector<float>> channelAudioData; // this is a vector of vectors of floats that stores audio data from different channels
};

/*
    audioProcess will take the audio and extract the audio in constructor (done)
    the extract function will return an array of sample stored in audioData (done)

    For each FFT_SIZE block in the audio sample, FFT_SIZE frequencies will be determined
    calculate the amplitude and frequency bin, apply Nyquist theorem and use only half of 
    the bin. Display the screen and calculate another FFT and shift the window upon display
    
    The return value of calculateSTFT() will be 
    hashMap(float, vector(float)) - The hashmap key = bin center frequency
    value will be vector of FFT amplitude
    Amplitude is the distance of the complex number

    11/12/2025
    Not going with the audio vizualisation that displays frequency over time instead amplitude over time
*/