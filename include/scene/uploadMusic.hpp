#pragma once
#include "buttons/upload.hpp"
#include "scene/scene.hpp"
#include "audio processing/audioFileHandling.hpp"

class UploadMusic : public Scene{
public:
    UploadMusic() = default;

    UploadButton* button;

    // Set all the scene object ready to be drawn
    void load(sf::RenderWindow& window) override{
        loadFont(font);
        
        winDim = {window.getSize().x , window.getSize().y};
        buttWindDim = {static_cast<float>(winDim.width), static_cast<float>(winDim.height)};

        button = new UploadButton(sf::Vector2f(buttWindDim.width, buttWindDim.height), font,
        uploadButtTxt, sf::Vector2f(buttDim.width, buttDim.height), buttTxtFontSize);
    }

    // Draw on window
    void render(sf::RenderWindow& window) override{
        button -> draw(window);
    };

    // Actions perfromed when specific scene object is clicked
    void clickActions(sf::RenderWindow& window) override{
        // button -> getTextObjectString() == uploadButtTxt
        if (button -> isClicked(window)){
            sf::WindowHandle windowHandle = window.getSystemHandle();
            AudioFileHandling audioPath;
            audioPath.openFileDir(windowHandle); // Opens file directory
            filePath = audioPath.filePath.get();
        }
    }

    // Actions perfromed when specific scene object is hovered
    void cursorActions(sf::RenderWindow& window, sf::RenderTarget& target) override{
        button -> updateColor(window);
        /* if (button -> getTextObjectString() == uploadButtTxt){
            button -> updateColor(window);
        } */
    }

    // boolean function that ensures when to move to next scene
    bool shouldMoveToNextScene() override{
        return (filePath.empty() == false);
        /* if (filePath.empty() == false){
            //std::cout<<"Received file"<<std::endl;
            return true;
        }
        return false; */
    }


    const std::string getFilePath() override{
        return filePath;
    }

    ~UploadMusic(){
        delete button;
    };

private:
    struct windowDim{
        unsigned int width, height;
    };
    struct buttonDim{
        float width, height;
    };
    windowDim winDim;
    buttonDim buttDim = {500.0f, 100.0f};
    buttonDim buttWindDim;
    std::string uploadButtTxt = "UPLOAD MUSIC", filePath;
    sf::Font font;
    int buttTxtFontSize = 50;
};