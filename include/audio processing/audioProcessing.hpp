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
    This class will extract two 512 frames at a time for visualization class. One 512 frame for
        current time and the other for future time. This applies for seeking as well.
*/
class AudioProcessing: public sf::Music{
    public:
        AudioProcessing(const std::string& filePath, uint16_t message):frames(1024){
            currentMusicDuration = sf::Time::Zero;
            lastExtractTime = sf::Time::Zero;
            quitControlMusicState = 4;
            typeOfVisual = message;

            loadAudioFile(filePath);
            queryDecoder();

            uint16_t clearBit = 0;
            messageQueue.push(message & clearBit);

            controlMusicState = std::thread(&AudioProcessing::controlLoop, this);//, std::ref(messageQueue), sampleBuffer, std::ref(currentMusicDuration));
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
            ma_result result = ma_decoder_init_file(filePath_, &config, &decoder);

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
            ma_result result = ma_data_source_get_data_format(&decoder, NULL, &channels, &sampleRate,
                channelMap, MA_MAX_CHANNELS);

            if (result != MA_SUCCESS) {
                throw std::runtime_error("Failed to retrieve data format");
            }

            maxSampleCount = frames * static_cast<ma_uint64>(channels);
            sampleBuffer = std::shared_ptr<float[]>(new float[maxSampleCount], std::default_delete<float[]>());//(maxSampleCount);
        }
        
        void extractChunks(){
            // If no time value is given start at 0
            void* tempBuffer = sampleBuffer.get();
            ma_uint64 framesRead = 0;
            ma_result result = ma_data_source_read_pcm_frames(&decoder, tempBuffer, frames, &framesRead);

            if (result != MA_SUCCESS) {
                throw std::runtime_error("Failed to read data");
            }

            /* std::cout<<"\nFrames: \n";
            [framesRead](std::shared_ptr<float []> arr){for(int i = 0; i<framesRead; ++i){std::cout<<arr[i]<<" ";}}(sampleBuffer); */
        }
        /*
            - Create two worker threads upon class instantiation to execute programe asynchronizely
            - Create a uint16 variable that stores the current state of music using bitwise
                operation
            - Threads: 1. Message driven thread that execute a program based on the event such as
                play, pause, seek, stop, seek, etc
                2. Time driven thread that handles PCM extraction, FFT and waveform preperation
        */
        
        void sendMusicSateToQueue(uint16_t message){
            messageQueue.push(message);
        }

        ~AudioProcessing(){
            messageQueue.push(quitControlMusicState);
            if(controlMusicState.joinable()){
                controlMusicState.join();
            }

            ma_decoder_uninit(&decoder);
        };

    private:
        const ma_uint64 frames; // Samples per channel
        ma_uint64 maxSampleCount; // FRAMES x CHANNELS
        std::shared_ptr<float[]> sampleBuffer; // pointer to an vector of interleaved samples
        
        // Miniaudio
        ma_uint32 sampleRate; // Hz or frames per second = 44100 or 48000
        ma_uint32 channels;
        ma_decoder decoder;
        ma_channel channelMap[MA_MAX_CHANNELS]; // maps out channel. MA_MAX_CHANNELS = 254

        // SFML
        sf::Music music;
        sf::Time currentMusicDuration, lastExtractTime;
        sf::Clock clock;
        uint16_t typeOfVisual, quitControlMusicState;

        // Threads
        /* Messages
            Entry: 0
            Music Stopped: 1 << 1
            Music Playing: 1 << 2
            Music Paused: 1 << 3
            Music Seek: 1 << 4
            Quit Thread: 1 << 5
        */
        ThreadSafeQueue<uint16_t> messageQueue;
        std::thread controlMusicState;
        
        void controlLoop(){//ThreadSafeQueue<uint16_t>& queue), std::shared_ptr<std::vector<float>> audioBuffer,
            //sf::Time& duration){
            const uint16_t MUSIC_STOPPED = sf::Music::Stopped;
            const uint16_t MUSIC_PAUSED = sf::Music::Paused;
            const uint16_t MUSIC_PLAYING = sf::Music::Playing;
            const uint16_t MUSIC_SEEK = 3;

            uint16_t message = 0, musicStopMessageCounter = 0;
            uint16_t& msgRef = message;

            while(true){
                messageQueue.waitAndPop(msgRef);

                if(message == MUSIC_STOPPED && currentMusicDuration == sf::Time::Zero){
                    if (musicStopMessageCounter >= 1){ continue;}
                    extractChunks();
                    ++musicStopMessageCounter;
                }
                else if(message == MUSIC_PLAYING || message == MUSIC_SEEK){
                    extractChunks();
                }
                else if(message == MUSIC_PAUSED){
                    continue;
                }
                else if(message == quitControlMusicState){
                    break;
                }
            }
        }
};

