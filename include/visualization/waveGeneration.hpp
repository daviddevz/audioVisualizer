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