#pragma once
#define MA_NO_FLAC

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <cstdint>
#include "SFML/Audio/Music.hpp"

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
    AudioProcessing(const std::string& filePath){loadAudioFile(filePath);};

    // Initialize decoder and pass the file path
    void loadAudioFile(const std::string& filePath) {
        // SFML
        if (!music.openFromFile(filePath)){
            throw std::runtime_error("filePath not found");
        }

        // Miniaudio
        const char* filePath_ = filePath.c_str(); // Convert std::string to C-style string
        result = ma_decoder_init_file(filePath_, NULL, &decoder);
        if (result != MA_SUCCESS) {
            throw std::runtime_error("Could not load file");
        }
    }

    // Music Streaming Operations
    sf::SoundSource::Status getMusicStatus(){return music.getStatus();}
    sf::Time getCurrenMusicDuration(){return currentMusicDuration;}
    void updateCurrentMusicDuration(sf::Time time){currentMusicDuration = time;}
    sf::Time getTotalMusicDuration(){return music.getDuration();}
    void playMusic(){music.play();};
    void pauseMusic(){music.pause();}; 
    void setMusicPlayingOffset () {music.setPlayingOffset(currentMusicDuration);};
    sf::Time restartClock(){return clock.restart();};

    // Query decoder for output format, sample rate and channel count
    void queryDecoder(){
        result = ma_data_source_get_data_format(&decoder, &format, &CHANNELS, &SAMPLE_RATE, channelMap,
        MA_MAX_CHANNELS);
        if (result != MA_SUCCESS) {
            throw std::runtime_error("Failed to retrieve data format");
        }

        // check if FRAMES and CHANNELS are initialized
        if (!(FRAMES > 0 && CHANNELS > 0)){
            throw std::runtime_error("Failed to initialize FRAMES and CHANNELS");
        }
        SAMPLE_COUNT = FRAMES * CHANNELS;

        // allocate adequate size for frameOut array
        if (format == ma_format_s16){
            framesOut = new int16_t[SAMPLE_COUNT];
        }
        else if (format == ma_format_f32){
            framesOut = new float[SAMPLE_COUNT];
        }
        else{
            throw std::runtime_error("Play 16 bit int or 32 bit float PCM audio");
        }

        sf::SoundStream::initialize(CHANNELS, SAMPLE_RATE);
    }

    sf::Int16* convert_f32_to_s16(std::size_t& sampleCount){
        sf::Int16* s16Ptr = new sf::Int16[sampleCount];
        float* pCopy = static_cast<float*>(framesOut); // specify the type of pointer in stack, assigns void ptr

        const float MAX_SCALE = 32767.0F;
        const float MIN_SCALE = -32768.0F;

        // To convert f32 to s16 multiply PCM data by 32767.0f
        for(int i = 0; i < sampleCount; i++){
            if (pCopy[i] >= 1.0F){
                s16Ptr[i] = static_cast<sf::Int16>(MAX_SCALE);
            }
            else if (pCopy[i] <= -1.0F){
                s16Ptr[i] = static_cast<sf::Int16>(MIN_SCALE);
            }
            else{
                s16Ptr[i] = static_cast<sf::Int16>(pCopy[i] * MAX_SCALE);
            }
        }
        s16PtrCopy = s16Ptr; // Create a copy of ptr to same address so it can be properly deallocated
        convert_f32_to_s16_flag = true;
        return s16Ptr;
    }

    sf::Int16* validateAudioFormat(void* pFramesOut, std::size_t& sampleCount){
        sf::Int16* tempFramesOut;
        if (format == ma_format_s16){
            tempFramesOut = static_cast<sf::Int16*>(pFramesOut);
        }
        else if (format == ma_format_f32){
            tempFramesOut = convert_f32_to_s16(sampleCount); // Assign address on stack
        }
        return tempFramesOut;
    }
    
    // Retrieve read samples and configure sound stream chunk in SFML format
    bool onGetData (Chunk &data) override{
        if (convert_f32_to_s16_flag){
            delete[] s16PtrCopy; // deallocate previous sample
            convert_f32_to_s16_flag = false;
        }

        uint64_t framesRead = 0; // Frames actually read
        /*
        pDecoder → your decoder instance
        pFrames → pointer to the buffer where PCM samples will be written (framesOut)
                1 Frame = N_Samples * Channels
        framesToRead → the maximum number of PCM frames you want (FRAMES)
        framesRead → output: the actual number of frames decoded (framesRead)
        */
        ma_result result = ma_decoder_read_pcm_frames(&decoder, framesOut, FRAMES, &framesRead);
        if (!(framesRead >= 0)){
            throw std::runtime_error("Decoder failed to read any frame");
        }
        data.sampleCount = framesRead * CHANNELS; // Frames might be different, so can't assign it once

        if (framesRead < FRAMES) {
            data.samples = validateAudioFormat(framesOut, data.sampleCount);
            return false;
        }
        data.samples = validateAudioFormat(framesOut, data.sampleCount);
        return true;
    }

    ~AudioProcessing(){
        //delete[] framesOut;

        /* if (convert_f32_to_s16_flag){
            delete[] s16PtrCopy; // deallocate last samples not deallocated in onGetData
        } */
        //delete[] audioChunk.samples; // delete allocated memory returned by convert_f32_to_s16(...)
        ma_decoder_uninit(&decoder);
    };

private:
    const uint16_t FRAMES = 1024; // Samples per channel
    uint64_t SAMPLE_COUNT; // FRAMES x CHANNELS

    // we don't know whether the output format is 16 bit int or 32 bit float array
    // We wil convert float array to 16 bit int array before passing it to sound stream chunk
    void* framesOut;
    sf::Int16* s16PtrCopy;
    bool convert_f32_to_s16_flag = false; 
    
    // Miniaudio
    ma_uint32 SAMPLE_RATE; // Hz or samples per second = 44100 or 48000
    ma_uint32 CHANNELS;
    ma_decoder decoder;
    ma_result result;
    ma_format format; // struct: ma_format_f32 = 5 (32 bit float), ma_format_s16 = 2 (16 bit int)
    ma_channel channelMap[MA_MAX_CHANNELS]; // maps out channel. MA_MAX_CHANNELS = 254

    // SFML
    sf::Music music;
    sf::Time currentMusicDuration = sf::Time::Zero;
    sf::Clock clock;
};

