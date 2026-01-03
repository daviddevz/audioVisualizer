#pragma once
#define MA_NO_FLAC

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <memory>
#include <thread>
#include <unordered_map>
#include <cmath>
#include "SFML/Audio/Music.hpp"
#include "stateUpdate.hpp"
#include "FFT/FFT.hpp"


extern "C" {
    #include "miniaudio/miniaudio.h"
}

/*
    * This class will stream audio from file for playing and decode audio for wave generation.
    * The music playing function will be tied to MusicPlayer class for music playing animation and 
        the decoding function will be used for wave generation of PCM data or frequency.
    * This class will extract two 512 frames at a time for visualization class. One 512 frame for
        current time and the other for future time. This applies for seeking as well.
__________________________________________________________________________________________________
Miniaudio audio extraction
    * Create two worker threads upon class instantiation to execute programe asynchronizely
    * Create a uint16 variable that stores the current state of music using bitwise operation
    * Threads: 1. Message driven thread that execute a program based on the event such as play,
        pause, seek, stop, seek, etc
        2. Time driven thread that handles PCM extraction, FFT and waveform preperation
*/

constexpr float cMinMagnitude = 1e-6f;

class AudioProcessing: public sf::Music{
    public:
        AudioProcessing(const std::string& filePath, uint16_t message, uint16_t quit, uint16_t seek){
            frames = 512;
            lastFramesRead = 0;
            maxMag = 0;
            currentMusicDuration = sf::Time::Zero;
            lastExtractedTime = sf::Time::Zero;
            QUIT = quit;
            MUSIC_SEEK = seek;
            typeOfVisual = message;

            loadAudioFile(filePath);
            queryDecoder();

            uint16_t clearBit = 0;
            messageStack.set(message & clearBit);

            controlMusicState = std::thread(&AudioProcessing::controlLoop, this);
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

            // FFT class 
            fft = std::make_unique<FFT_<float>>();
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
            ma_result result = ma_data_source_get_data_format(&decoder, NULL, &channels, &sampRate,
                channelMap, MA_MAX_CHANNELS);

            if (result != MA_SUCCESS) {
                throw std::runtime_error("Failed to retrieve data format");
            }

            maxSampleCount = frames * static_cast<ma_uint64>(channels);
            sampleBuffer = std::shared_ptr<float[]>(new float[maxSampleCount],
                std::default_delete<float[]>());
        }
        
        void extractChunks(uint16_t musicState){
            void* tempBuffer = sampleBuffer.get();
            ma_uint64 fRead = 0;
            ma_result result;

            if (musicState == sf::Music::Playing || musicState == MUSIC_SEEK){
                if (((lastExtractedTime.asSeconds() - static_cast<float>(lastFramesRead)/sampRate)
                    <= currentMusicDuration.asSeconds()) ||
                    (lastExtractedTime.asSeconds() >= currentMusicDuration.asSeconds())){

                    if(currentMusicDuration.asSeconds() > music.getDuration().asSeconds()){
                        currentMusicDuration = music.getDuration();
                    }

                    result = ma_data_source_seek_to_second(&decoder, currentMusicDuration.asSeconds());
                    if (result != MA_SUCCESS) {
                        if(result != MA_AT_END){
                            throw std::runtime_error("Failed to seek new time location");
                        }
                    }

                    result = ma_data_source_read_pcm_frames(&decoder, tempBuffer, frames, &fRead);
                    if (result != MA_SUCCESS) {
                        if(result != MA_AT_END){
                            throw std::runtime_error("Failed to read data");
                        }
                    }

                    lastExtractedTime = sf::seconds(currentMusicDuration.asSeconds()
                        + static_cast<float>(lastFramesRead)/sampRate);
                    lastFramesRead = fRead;
                }
            }

            else if (musicState == sf::Music::Stopped){
                result = ma_data_source_read_pcm_frames(&decoder, tempBuffer, frames, &fRead);
                if (result != MA_SUCCESS) {
                    if(result != MA_AT_END){
                        throw std::runtime_error("Failed to read data");
                    }
                }

                lastExtractedTime = sf::seconds(currentMusicDuration.asSeconds()
                    + static_cast<float>(lastFramesRead)/sampRate);
                lastFramesRead = fRead;
            }

            mixDownChannels();

            /* [fRead](int channels, std::shared_ptr<float[]> buffer) -> void{
                for(int i = 0; i< fRead*channels; ++i){std::cout<<i<<": "<<buffer[i]<<std::endl;}
            }(channels, sampleBuffer); */
        }

        void mixDownChannels(){
            //mixDownBuffer.clear();
            std::vector<float> tempMixDownBuffer = {};
            maxMag = cMinMagnitude; // division by 0 could be a problem;
            for (int i = 0; i < lastFramesRead; i += channels){
                float addBuffer = 0;

                for(int j = 0; j < channels; ++j){
                    addBuffer += sampleBuffer[i + j];
                    //std::cout<<" "<<i <<" Add Buffer: "<<addBuffer <<" Sample Buffer: "<<sampleBuffer[i + j];//<<std::endl;
                }
                float avgBuffer = addBuffer / channels;
                tempMixDownBuffer.push_back(avgBuffer);
                //std::cout<<" Mixed Down Buffer: "<<mixDownBuffer[i/channels] <<std::endl;
                if(maxMag < std::fabs(avgBuffer)){
                    maxMag = std::fabs(avgBuffer);
                }
            }

            mixDownBuffer = tempMixDownBuffer;
            //fft -> runFFT(mixDownBuffer, static_cast<unsigned>(lastFramesRead));
        }

        void sendMusicStateUpdate(uint16_t message){ messageStack.set(message);}
        uint64_t getFrames() const{ return frames;}
        std::vector<float> getMixDownBuffer(){ return mixDownBuffer;}
        float getMaxMag(){ return maxMag;}

        ~AudioProcessing(){
            messageStack.set(QUIT);
            if(controlMusicState.joinable()){
                controlMusicState.join();
            }

            ma_decoder_uninit(&decoder);
        };

    private:
        ma_uint64 frames; // Samples per channel
        ma_uint64 lastFramesRead;
        ma_uint64 maxSampleCount; // FRAMES x CHANNELS
        std::shared_ptr<float[]> sampleBuffer; // pointer to an vector of interleaved samples
        std::vector<float> mixDownBuffer;
        float maxMag;

        // Miniaudio
        ma_uint32 sampRate; // Hz or frames per second = 44100 or 48000
        ma_uint32 channels;
        ma_decoder decoder;
        ma_channel channelMap[MA_MAX_CHANNELS]; // maps out channel. MA_MAX_CHANNELS = 254

        // SFML
        sf::Music music;
        sf::Time currentMusicDuration, lastExtractedTime;
        sf::Clock clock;
        uint16_t typeOfVisual, QUIT, MUSIC_SEEK;

        // FFT
        std::unique_ptr<FFT_<float>> fft;

        // Threads
        /* Messages
            Music Stopped: 0, Music Playing: 1, Music Paused: 2
        */
        StateUpdate<uint16_t> messageStack;
        std::thread controlMusicState;
        
        void controlLoop(){
            const uint16_t MUSIC_STOPPED = sf::Music::Stopped;
            const uint16_t MUSIC_PAUSED = sf::Music::Paused;
            const uint16_t MUSIC_PLAYING = sf::Music::Playing;

            uint16_t message = 0, stopMsgCounter = 0;
            uint16_t& msgRef = message;

            while(true){
                messageStack.waitAndGet(msgRef);

                if(message == MUSIC_STOPPED && currentMusicDuration == sf::Time::Zero){
                    if (stopMsgCounter >= 1){ continue;}
                    extractChunks(message);
                    ++stopMsgCounter;
                }

                else if(message == MUSIC_PLAYING || message == MUSIC_SEEK){
                    extractChunks(message);}

                else if(message == MUSIC_PAUSED){
                    continue;
                }

                else if(message == QUIT){
                    break;
                }
            }
        }
};

