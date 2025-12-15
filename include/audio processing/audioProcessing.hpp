#pragma once
#define MA_NO_FLAC

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <memory>
#include <thread>
#include "SFML/Audio/Music.hpp"
#include "ThreadSafeQueue.hpp"

extern "C" {
    #include "miniaudio/miniaudio.h"
}

/*
    This class will stream audio from file for playing and decode audio for wave generation.
    The music playing function will be tied to MusicPlayer class for music playing animation and 
    the decoding function will be used for wave generation of PCM data or frequency.
*/
class AudioProcessing: public sf::Music{
    public:
        AudioProcessing(const std::string& filePath, uint16_t message){
            loadAudioFile(filePath);
            queryDecoder();
            messageQueue.push(message); //Test

            controlMusicState = std::thread(&AudioProcessing::controlLoop, this,
                std::ref(messageQueue), sampleBuffer, std::ref(currentMusicDuration));
        }

        // Initialize decoder and pass the file path
        void loadAudioFile(const std::string& filePath) {
            // SFML
            if (!music.openFromFile(filePath)){
                throw std::runtime_error("filePath not found");
            }

            // Miniaudio
            const char* filePath_ = filePath.c_str();

            // only set output format but keeps default settings for channels and sample rate
            ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 0, 0);
            result = ma_decoder_init_file(filePath_, &config, &decoder);

            if (result != MA_SUCCESS) {
                throw std::runtime_error("Could not load file");
            }
        }

        // SFML Music Streaming Operations
        sf::SoundSource::Status getMusicStatus(){return music.getStatus();}
        sf::Time getCurrenMusicDuration(){return currentMusicDuration;}
        void updateCurrentMusicDuration(sf::Time time){currentMusicDuration = time;}
        sf::Time getTotalMusicDuration(){return music.getDuration();}
        void playMusic(){music.play();};
        void pauseMusic(){music.pause();}; 
        void setMusicPlayingOffset () {music.setPlayingOffset(currentMusicDuration);};
        sf::Time restartClock(){return clock.restart();};

        // Miniaudio
        void queryDecoder(){
            result = ma_data_source_get_data_format(&decoder, NULL, &channels, &sampleRate,
                channelMap, MA_MAX_CHANNELS);

            if (result != MA_SUCCESS) {
                throw std::runtime_error("Failed to retrieve data format");
            }

            maxSampleCount = frames * static_cast<ma_uint64>(channels);
            sampleBuffer = std::make_shared<std::vector<float>>(maxSampleCount);
        }
        
        void extractChunks(sf::Time time = sf::seconds(0.0f)){
            // If no time value is given start at 0
            void* tempBuffer = sampleBuffer.get();
            ma_uint64 framesRead = 0;
            result = ma_data_source_read_pcm_frames(&decoder, tempBuffer, frames, &framesRead);

            if (result != MA_SUCCESS) {
                throw std::runtime_error("Failed to read data");
            }
        }
        /*
            - Create two worker threads upon class instantiation to execute programe asynchronizely
            - Create a uint16 variable that stores the current state of music using bitwise
                operation
            - Threads: 1. Message driven thread that execute a program based on the event such as
                play, pause, seek, stop, seek, etc
                2. Time driven thread that handles PCM extraction, FFT and waveform preperation
        */
        
        void pushMessageToQueue(uint16_t message){
            messageQueue.push(message);
        }

        ~AudioProcessing(){
            messageQueue.push(quit);
            if(controlMusicState.joinable()){
                controlMusicState.join();
            }

            ma_decoder_uninit(&decoder);
        };

    private:
        const ma_uint64 frames = 1024; // Samples per channel
        ma_uint64 maxSampleCount; // FRAMES x CHANNELS
        std::shared_ptr<std::vector<float>> sampleBuffer; // pointer to an vector of interleaved samples
        
        // Miniaudio
        ma_uint32 sampleRate; // Hz or frames per second = 44100 or 48000
        ma_uint32 channels;
        ma_decoder decoder;
        ma_result result;
        ma_channel channelMap[MA_MAX_CHANNELS]; // maps out channel. MA_MAX_CHANNELS = 254

        // SFML
        sf::Music music;
        sf::Time currentMusicDuration = sf::Time::Zero;
        sf::Clock clock;

        // Threads
        /* Messages
            Waveform: 0
            Spectrogram: 1
            Music Stopped: 1 << 1
            Music Playing: 1 << 2
            Music Paused: 1 << 3
            Music Seek: 1 << 4
            Quit Thread: 1 << 5
        */
        ThreadSafeQueue<uint16_t> messageQueue;
        std::thread controlMusicState;
        uint16_t quit = 1 << 5;

        void controlLoop(ThreadSafeQueue<uint16_t>& queue, std::shared_ptr<std::vector<float>> audioBuffer,
            sf::Time& duration){
            const uint16_t WAVE_MUSIC_STOP = 1 << 1;
            const uint16_t WAVE_MUSIC_PLAY = 1 << 2;
            const uint16_t WAVE_MUSIC_PAUSE = 1 << 3;
            const uint16_t WAVE_MUSIC_SEEK = 1 << 4;
            const uint16_t QUIT = 1 << 5;
            const uint16_t SPECT_MUSIC_STOP = 1 | 1 << 1;
            const uint16_t SPECT_MUSIC_PLAY = 1 | 1 << 2;
            const uint16_t SPECT_MUSIC_PAUSE = 1 | 1 << 3;
            const uint16_t SPECT_MUSIC_SEEK = 1 | 1 << 4;

            uint16_t message = 0;
            uint16_t& msgRef = message;

            while(true){
                queue.waitAndPop(msgRef);

                switch (message){
                /* WAVEFORM CASES*/
                    case (WAVE_MUSIC_STOP):
                        /* code */
                        break;
                    case (WAVE_MUSIC_PLAY):
                        /* code */
                        break;
                    case (WAVE_MUSIC_PAUSE):
                        /* code */
                        break;
                    case (WAVE_MUSIC_SEEK):
                        /* code */
                        break;

                /* SPECTROGRAM CASES*/
                    case (SPECT_MUSIC_STOP):
                        /* code */
                        break;
                    case (SPECT_MUSIC_PLAY):
                        /* code */
                        break;
                    case (SPECT_MUSIC_PAUSE):
                        /* code */
                        break;
                    case (SPECT_MUSIC_SEEK):
                        /* code */
                        break;

                /* Quit CASE*/
                    case (QUIT):
                        break;

                    default:
                        // Extract from default time
                        if (message){
                            /* SPECTROGRAM CASES*/
                            std::cout<<"Test work for spectrogram\n";
                        }
                        else{
                            /* WAVEFORM CASES*/
                            std::cout<<"Test work for waveform\n";
                        }
                        break;
                    }
                
                if (message & QUIT){
                    break;
                }
            }
        }
};

