#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <complex>
#include "FFT.hpp"

class AudioProcessing {
public:
    FFT* STFT;
    ProgressBar* progressBar;
    RenderText* renderText;
    bool isProcessingCompleted = false;

    AudioProcessing(const std::string& filePath_, ProgressBar*& progressBar_, RenderText*& renderText_,
    sf::RenderWindow& window_):filePath(filePath_), progressBar(progressBar_), renderText(renderText_),
    window(&window_){
        STFT = new FFT();

        loadAudioFile();

        processingStage = "Extracting Audio Sample ...";
        renderProgress();
        audioData = extractAudioSample();

        completionRate = 0.0f;
        processingStage = "Fast Fourier Transform ...";
        renderProgress();
        FFTResult = calculateSTFT();
    }; 
    
    // Extract audio sample
    std::vector<float> extractAudioSample(){
        completionRate = 0.0f; // It will be update later in the code
    };

    // STFT - Short Time Fourier Transform
    // The size of the hashMap will be equal to FFT_SIZE
    // The size of the vector will be (total audio sample / FFT_SIZE) rounded to nearest integer
    std::unordered_map<float, std::vector<float>> calculateSTFT(){
    };

    std::unordered_map<std::string, float> getProcessingProgress(){
        std::unordered_map<std::string, float> progress;
        progress[processingStage] = completionRate;
        return progress;
    }

    void renderProgress(){
        renderText -> setStr(processingStage);
        progressBar -> updateProgressBar(completionRate);
        progressBar -> draw(*window);
    }

    std::unordered_map<float, std::vector<float>> getFFTResult(){
        return FFTResult;
    }

    ~AudioProcessing(){};

private:
    std::string filePath, processingStage = "";
    const int SAMPLE_RATE = 44100; // Hz or sample/sec
    const int FFT_SIZE = 1024; // 1024 audio samples considered per FFT
    std::vector<float> audioData;
    std::unordered_map<float, std::vector<float>> FFTResult;
    float completionRate = 0.0f;
    sf::Music audio;
    sf::RenderWindow* window;

    void loadAudioFile(){

    }; 
};

/*
    audioProcess will take the audio and extract the audio in constructor
    the extract function will return an array of sample stored in audioData

    For each FFT_SIZE block in the audio sample, FFT_SIZE frequencies will be determined
    The return value of calculateSTFT() will be 
    hashMap(float, vector(float)) - The hashmap key = bin center frequency
    value will be vector of FFT amplitude
    Amplitude is the distance of the complex number
*/