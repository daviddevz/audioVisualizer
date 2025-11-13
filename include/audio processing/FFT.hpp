/* #include <iostream>
#include <vector>
#include <complex>

#define _USE_MATH_DEFINES
#include <cmath>

class FFT{
private:
public:
    FFT(const std::vector<float>& timeDomainSignal, const unsigned int fftSize):SIGNAL(timeDomainSignal),
    FFT_SIZE(fftSize){};

    // Recursive function to perform the Cooley-Tukey FFT
    std::vector<std::complex<float>> fftRecursive(const std::vector<float>& x) {
        int N = x.size();
        if (N == 1) {
            return {std::complex<float>(x[0], 0)}; 
        }

        std::vector<float> even(N / 2), odd(N / 2); 

        for (int i = 0; i < N / 2; ++i) {
            even[i] = x[2 * i];
            odd[i] = x[2 * i + 1];
        }

        std::vector<std::complex<float>> yEven = fftRecursive(even);
        std::vector<std::complex<float>> yOdd = fftRecursive(odd);

        std::vector<std::complex<float>> y(N);

        for (int k = 0; k < N / 2; ++k) {
            float angle = -2 * M_PI * k / N;
            std::complex<float> w(cos(angle), sin(angle));
            y[k] = yEven[k] + w * yOdd[k];
            y[k + N / 2] = yEven[k] - w * yOdd[k];
        }

        return y;
    }

    // Function to calculate the DFT for all bins with specified FFT size
    std::vector<float> calculate_dft() {
        // Zero-padding
        std::vector<float> paddedSignal(FFT_SIZE, 0.0); 
        copy(SIGNAL.begin(), SIGNAL.end(), paddedSignal.begin()); 
        
        // Calculate DFT using recursive FFT
        std::vector<std::complex<float>> dftValues = fftRecursive(paddedSignal);
        std::vector<float> dftMag;

        for (std::complex<float> val: dftValues){
            float mag = sqrtf(val.real()*val.real() + val.imag()*val.imag());
            dftMag.push_back(mag);
        }
        return dftMag;
    }

    ~FFT() = default;
private:
    const std::vector<float> SIGNAL;
    const unsigned int FFT_SIZE;
};

 */