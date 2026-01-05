#pragma once
#define __USE_SQUARE_BRACKETS_FOR_ELEMENT_ACCESS_OPERATOR 1

#include <vector>
#include <bit>
#include "SimpleFFT/fft.h"

class FFT_{
    public:
        FFT_() = default;

        bool isPowerOfTwo(){
            return (nFFT != 0) && (nFFT & (nFFT-1)) == 0;
        }

        void padZeros(){
            kPow += 1;
            nFFTPad = (1 << kPow);
            fftSamplesIn.resize(nFFTPad, 0);
        }

        // Find largest integer k such that 2^k <= nFFT
        // std::numeric_limits<unsigned>::digits gives the number of value bits in an unsigned
        // std::countl_zero(n) counts the number of leading zero bits in nFFT
        // Subtract by 1 because bit positions are zero-based index, while digist is a count
        unsigned floorLog2(){
            unsigned int digit = std::numeric_limits<unsigned int>::digits;

            if (nFFT == 0){ return 0;}

            auto countl_zero = [digit](unsigned int size){
                unsigned int k = 0;
                
                if(size == 0){ return digit;}

                while (size != 0){
                    size >>= 1;
                    ++k;
                }
                return digit - k;
            };

            return digit - 1 - countl_zero(nFFT);
        }

        void runFFT(std::vector<std::complex<double>>* samples, unsigned int size){
            fftSamplesIn = *samples;
            nFFT = size;

            if (!isPowerOfTwo()){
                kPow = floorLog2();
                padZeros();
            }
            else{
                nFFTPad = nFFT;
            }

            // FFT implementation using simpleFFT
            fftSamplesOut.resize(nFFTPad);
            const char* error = NULL;
            bool result = simple_fft::FFT(fftSamplesIn, fftSamplesOut, nFFTPad, error);
            if (!result){throw std::runtime_error(error);};

            *samples = fftSamplesOut;
        }

        ~FFT_() = default;

    private:
        unsigned nFFT, nFFTPad, kPow;
        std::vector<std::complex<double>> fftSamplesIn, fftSamplesOut;
};