#pragma once
#include "buttons/upload.hpp"
#include "scene/scene.hpp"

class SelectVisual : public Scene{
public:
    SelectVisual() = default;

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
        // && waveform -> getTextObjectString() == buttTxt[0]
        if (waveform -> isClicked(window)){
            /*sf::WindowHandle windowHandle = window.getSystemHandle();
            AudioFileHandling audioPath;
            audioPath.openFileDir(windowHandle); // Opens file directory
            filePath = audioPath.filePath.get(); */
            //std::cout<<"waveform"<<std::endl;
            typeOfVisual = buttTxt[0];
        }

        // && spectrogram -> getTextObjectString() == buttTxt[1]
        else if (spectrogram -> isClicked(window)){
            //std::cout<<"spectrogram"<<std::endl;
            typeOfVisual = buttTxt[1];
        }
    }

    // Actions perfromed when specific scene object is hovered
    void cursorActions(sf::RenderWindow& window, sf::RenderTarget& target) override{
        waveform -> updateColor(window);
        spectrogram -> updateColor(window);
        /* if (waveform -> getTextObjectString() == buttTxt[0]){
            waveform -> updateColor(window);
        }

        else if (spectrogram -> getTextObjectString() == buttTxt[1]){
            spectrogram -> updateColor(window);
        } */
    }

    // boolean function that ensures when to move to next scene
    bool shouldMoveToNextScene() override{
        //std::cout<<"Is typeOfVisual empty? "<<typeOfVisual.empty()<<std::endl;
        return (typeOfVisual.empty() == false);
        /* if (typeOfVisual.empty() == false){
            return true;
        }
        return false; */
    }

    // Returns next sceneId after retrieving audio filepath
    /* std::string getNextSceneId(std::vector<std::string>& sceneIds, std::string& sceneId) override{
        if (filePath.empty() == false){
            std::vector<std::string>::iterator it = std::find(sceneIds.begin(), sceneIds.end(), sceneId);

            if (it != sceneIds.end() && std::next(it) != sceneIds.end()){
                return *(it+1); // deference the iterator
            }
            return ""; // this code will never execute because uploadMusic is the first scene
        }
        return "";
    } */

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
    buttonDim buttDim[2] = {{400.0f, 100.0f}, {500.0f, 100.0f}};
    buttonDim buttWindDim;
    std::string buttTxt[2] = {"WAVEFORM", "SPECTROGRAM"};
    std::string typeOfVisual;
    sf::Font font;
    int buttTxtFontSize = 50;
};