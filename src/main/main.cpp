/* Win32 includes*/
#include <windows.h>
#include <commdlg.h>

/* C++ includes*/
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

/* Project includes*/
#include "button.hpp"
#include "progressBar.hpp"
#include "audioFileHandling.hpp"

/* SFML includes */
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>

int main()
{
    unsigned int windowWidth = 2000, windowHeight = 1000;
    float buttonWidth = 500, buttonHeight = 100;
    int fontSize = 50;
    std::string uploadButtonText = "UPLOAD MUSIC", windowTitle = "Audio Visualizer";
    

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), windowTitle,
    sf::Style::Titlebar | sf::Style::Close);

    // Font
    sf::Font font;
    if(!font.loadFromFile("C:\\SFML-2.6.1\\Arial Font\\ARIAL.TTF")){
        return 1;
    }

    // Create button object
    float windowWidthFloat = static_cast<float>(windowWidth);
    float windowHeightFloat = static_cast<float>(windowHeight);

    UploadButton myUploadButton(sf::Vector2f(windowWidthFloat, windowHeightFloat), 
    font, uploadButtonText, sf::Vector2f(buttonWidth, buttonHeight), fontSize);
    
    // Create progress bar object
    float loadingStages = 5;
    ProgressBar loadingBar(loadingStages, sf::Vector2f(windowWidthFloat, windowHeightFloat),
    sf::Vector2f(buttonWidth, (buttonHeight * 0.5)));

    // Start the game loop
    while (window.isOpen())
    {
        ShowWindow(GetConsoleWindow(), SW_HIDE); // Hide console window

        sf::Event event;// Process events
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){ // Close window: exit
                window.close();
            }

            // Check for button clicks
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
                if (myUploadButton.isClicked(window) && myUploadButton.clearButton == false){
                    myUploadButton.clearButton = true;
                    sf::WindowHandle windowHandle = window.getSystemHandle();
                    AudioFileHandling myAudioFile;
                    myAudioFile.openFileDir(windowHandle); // Opens file directory
                    //std::cout<<"File Path in main.cpp"<<ouputFilePath<<std::endl;
                }
            }

            else if (event.type == sf::Event::MouseMoved){
                myUploadButton.updateColor(window); //update button color if mouse hover over button
            }
        }
 
        window.clear(sf::Color(111, 143, 175)); // Clear screen with Denim color

        // Conditional statement that controls how button is displayed on screen
        if (myUploadButton.clearButton == false){
            myUploadButton.draw(window); // Draws button
        }
        else if (loadingBar.clearProgressBar == false){
            sf::Color backgroundColor(240, 255, 255);
            sf::Color maskedColor(0, 255, 255, 50);
            // backgroundShape Design
            loadingBar.shapeDesign(backgroundColor, maskedColor);

            // setup text
            loadingBar.setText(font, (fontSize - 20));

            loadingBar.updateProgressBar(2, "Processing ...");
            loadingBar.draw(window);
            //loadingBar.clearProgressBar = true;
        }
        
        window.display(); // Update the window
    }
 
    return 0;
}