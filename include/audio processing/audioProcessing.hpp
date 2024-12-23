#pragma once

#include <vector>
#include <string>
#include <complex>
#include "audioFileHandling.hpp"

class AudioProcessing {
public:
    AudioProcessing(){}; 

    void setFilePath (const std::string& filePath_){
        filePath = filePath_;
    }
    
    std::vector<std::vector<std::complex<double>>> calculateSTFT();
    ~AudioProcessing(){};

private:
    std::string filePath;
    const int sampleRate = 44100; 
    const int windowSize = 1024; 
    const int hopSize = 512; 
    std::vector<float> audioData_; 

    void loadAudioFile(); 
};