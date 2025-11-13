#pragma once
#include "SFML/Graphics.hpp"

class WaveGeneration{
private:
    sf::Vector2u windowDim_;
    sf::Vector2f initialPos_; // Bottom position of the wave
    const unsigned int numOfWaves;
    sf::RectangleShape rect;
    float magnitude, frequency;
public:
    WaveGeneration(sf::Vector2u windowDim, const unsigned int FFTSampleSize, float mag, float freq,
    sf::Vector2f initialPos):windowDim_(windowDim), numOfWaves(FFTSampleSize), magnitude(mag), frequency(freq),
    initialPos_(initialPos){};

    void designWave(){
        float waveWidth = (static_cast<float>(windowDim_.x)/numOfWaves) * 0.95f;
        float waveHeight = (static_cast<float>(windowDim_.x) - initialPos_.y) * magnitude;

    }
    sf::RectangleShape getWave(){
        return rect;
    }

};


/* 
    * This class takes window size and FFTRenderSize to build a wave(rectangle) that can be displayed
    * The wave color and height will be updated based on FFT mag and frequency bin
    * The size of the recetangle will be based of the window dimension
 */