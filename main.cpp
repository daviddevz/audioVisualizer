#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <string>
#include "Button.hpp"

int main()
{
    unsigned int windowWidth = 2000, windowHeight = 1000;
    float buttonWidth = 500, buttonHeight = 100;
    int fontSize = 50;
    std::string uploadButtonText = "UPLOAD MUSIC", windowTitle = "Audio Visualizer";
    

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), windowTitle, sf::Style::Titlebar | sf::Style::Close);

    // Font
    sf::Font font;
    if(!font.loadFromFile("C:\\SFML-2.6.1\\Arial Font\\ARIAL.TTF")){
        return 1;
    }

    // Create button object
    float windowWidthFloat = static_cast<float>(windowWidth);
    float windowHeightFloat = static_cast<float>(windowHeight);
    UploadButton myUploadButton(sf::Vector2f(windowWidthFloat, windowHeightFloat), font, uploadButtonText, sf::Vector2f(buttonWidth, buttonHeight), fontSize);

    // Start the game loop
    while (window.isOpen())
    {
        //ShowWindow(GetConsoleWindow(), SW_HIDE); // Hide console window
        sf::Event event;// Process events
        while (window.pollEvent(event))
        {
            
            if (event.type == sf::Event::Closed){ // Close window: exit
                window.close();
            }

            // Check for button clicks
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
                if (myUploadButton.isClicked(window)){
                    myUploadButton.openFileDir();
                    break; // Exit the event loop after handling the first click
                }
            }

            else if (event.type == sf::Event::MouseMoved){
                myUploadButton.updateColor(window); //update button color if mouse hover over button
            }
        }
 
        window.clear(); // Clear screen 
        myUploadButton.draw(window); // Draws button
        
        window.display(); // Update the window
    }
 
    return 0;
}