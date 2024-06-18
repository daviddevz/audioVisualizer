#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <windows.h>
#include <iostream>
#include <string>
#include "Button.hpp"

unsigned int windowWidth = 2000;
unsigned int windowHeight = 1000;
float buttonWidth = 500;
float buttonHeight = 100;
int fontSize = 50;
std::string uploadButtonText = "UPLOAD MUSIC";



int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Audio Visualizer");

    // Font
    sf::Font font;
    if(!font.loadFromFile("C:\\SFML-2.6.1\\Arial Font\\ARIAL.TTF")){
        return 1;
    }

    // Create button object
    float windowWidthFloat = static_cast<float>(windowWidth);
    float windowHeightFloat = static_cast<float>(windowHeight);
    UploadButton myUploadButton(sf::Vector2f(windowWidth, windowHeight), font, uploadButtonText, sf::Vector2f(buttonWidth, buttonHeight), fontSize);

    // Start the game loop
    while (window.isOpen())
    {
        ShowWindow(GetConsoleWindow(), SW_HIDE);  // Hide console window
        sf::Event event;// Process events
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed){
                window.close();
            }
        }
 
        window.clear(); // Clear screen 
        myUploadButton.draw(window); // Draws button
        window.display(); // Update the window
    }
 
    return 0;
}