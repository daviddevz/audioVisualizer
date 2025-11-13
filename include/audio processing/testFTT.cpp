/* #include <iostream>
#include <vector>
#include "FFT.hpp"

int main() {
    
    // Example usage
    std::vector<float> time_domain_signal = {0.0f, 1.0f, 0.0f, -1.0f}; // Example signal
    float sampleRate = 44100.0; // Example sample rate
    int fftSize = 4; // Example FFT size (power of 2)

    FFT fft(time_domain_signal, fftSize);
    std::vector<float> dftValues = fft.calculate_dft();

    // Calculate bin width
    double bin_width = sampleRate / fftSize; 

    // Print the DFT values for all bins
    for (int k = 0; k < fftSize; ++k) {
        double frequency = k * bin_width;
        std::cout << "Bin " << k << ": Frequency " << frequency << " Hz, DFT Mag: " << dftValues[k] << std::endl;
    }

    return 0;
} */