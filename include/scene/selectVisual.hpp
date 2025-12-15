#pragma once
#include "buttons/upload.hpp"
#include "scene/scene.hpp"
#include <array>

class SelectVisual : public Scene{
public:
    SelectVisual(){
        buttDim = {{{400.0f, 100.0f}, {500.0f, 100.0f}}};
        buttTxt = {"WAVEFORM", "SPECTROGRAM"};
    };

    UploadButton* waveform;
    UploadButton* spectrogram;

    // Set all the scene object ready to be drawn
    void load(sf::RenderWindow& window) override{
        loadFont(font);
        
        winDim = {window.getSize().x , window.getSize().y};
        buttWindDim = {static_cast<float>(winDim.width), static_cast<float>(winDim.height)};
       
        waveform = new UploadButton(sf::Vector2f(buttWindDim.width, buttWindDim.height), font,
        buttTxt[0], sf::Vector2f(buttDim[0].width, buttDim[0].height), buttTxtFontSize);

        spectrogram = new UploadButton(sf::Vector2f(buttWindDim.width, buttWindDim.height), font,
        buttTxt[1], sf::Vector2f(buttDim[1].width, buttDim[1].height), buttTxtFontSize);

        waveform -> moveButton(-500, 0);
        spectrogram -> moveButton(500, 0);
    }

    // Draw on window
    void render(sf::RenderWindow& window) override{
        waveform -> draw(window);
        spectrogram -> draw(window);
    };

    // Actions perfromed when specific scene object is clicked
    void clickActions(sf::RenderWindow& window) override{
        if (waveform -> isClicked(window)){
            typeOfVisual = buttTxt[0];
        }
        else if (spectrogram -> isClicked(window)){
            typeOfVisual = buttTxt[1];
        }
    }

    // Actions perfromed when specific scene object is hovered
    void cursorActions(sf::RenderWindow& window, sf::RenderTarget& target) override{
        waveform -> updateColor(window);
        spectrogram -> updateColor(window);
    }

    // boolean function that ensures when to move to next scene
    bool shouldMoveToNextScene() override{
        return (typeOfVisual.empty() == false);
    }

    const std::string getTypeOfVisual() override{
        return typeOfVisual;
    }

    ~SelectVisual(){
        delete waveform;
        delete spectrogram;
    };

private:
    struct windowDim{
        unsigned int width, height;
    };

    struct buttonDim{
        float width, height;
    };

    windowDim winDim;
    std::array<buttonDim, 2> buttDim;
    buttonDim buttWindDim;
    std::array<std::string, 2> buttTxt;
    std::string typeOfVisual;
    sf::Font font;
    int buttTxtFontSize = 50;
};