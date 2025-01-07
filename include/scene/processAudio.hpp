#pragma once
#include "scene/scene.hpp"
#include "visualization/progressBar.hpp"
#include "visualization/renderText.hpp"
#include "SFML/Graphics.hpp";
#include "audio processing/audioProcessing.hpp"

class ProcessAudio : public Scene{
public:
    ProgressBar* progressBar;
    RenderText* renderText;
    AudioProcessing* audioProcessing;
    
    ProcessAudio() = default;

    void load(sf::RenderWindow& window) override{
        sf::Vector2f windowDimension = {static_cast<float>(window.getSize().x),
                                        static_cast<float>(window.getSize().y)};
        sf::Vector2f shapeDimension = {1000.0f, 200.0f};
        sf::Vector2f shapePosition = {windowDimension.x/2.0f - shapeDimension.x/2.0f,
                                    windowDimension.y /2.0f - shapeDimension.y/2.0f};
        sf::Color backgroundColor = sf::Color::White;
        sf::Color maskedColor = sf::Color::Blue;

        progressBar = new ProgressBar(shapePosition, backgroundColor, maskedColor, shapeDimension);

        if(!font.loadFromFile("C:\\SFML-2.6.1\\Arial Font\\ARIAL.TTF")){
            throw std::runtime_error("Failed to load font");
        }

        unsigned int charSize = 32u;
        sf::Vector2f textPosition = {shapePosition.x, shapePosition.y * 0.9f};
        renderText = new RenderText(font, "", backgroundColor, textPosition, charSize);

        audioProcessing = new AudioProcessing(filePath, progressBar, renderText, window);
    };

    void render(sf::RenderWindow& window) override{};
    void clickActions(sf::RenderWindow& window) override{};
    void cursorActions(sf::RenderWindow& window, sf::RenderTarget& target) override{};
    
    bool shouldMoveToNextScene() override{
        return audioProcessing -> isProcessingCompleted;
    };

    void setFilePath(const std::string& filePath_) override {
        filePath = filePath_;
    }

    std::unordered_map<float, std::vector<float>> getFFTResult(){
        return audioProcessing -> getFFTResult();
    }
    
    ~ProcessAudio(){
        delete progressBar;
        delete renderText;
        delete audioProcessing;
    };

private:
    std::string filePath;
    sf::Font font;
    
};

/*
    load function can setup progressBar and audioProcessing
    render() will render the progress for extracting audio file and calculating STFT
*/ 