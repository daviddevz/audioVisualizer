#pragma once

#include <vector>
#include <string>
#include "SFML/Graphics.hpp"
#include "audio processing/audioProcessing.hpp"

class Visualization {
public:
    Visualization(const std::string& filePath, const std::vector<std::vector<std::complex<double>>>& stftData); 

    void visualize(sf::RenderWindow& window); 

private:
    const std::string filePath_;
    const std::vector<std::vector<std::complex<double>>> stftData_; 
    const int videoFps = 30; 
    // ... other private members (e.g., for storing visualization data, rendering)

    void downsampleSTFT(); 
    void renderSpectrum(sf::RenderWindow& window, const std::vector<std::complex<double>>& spectrum); 
    // ... other private helper functions
};