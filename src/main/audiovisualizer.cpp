// Written by David I. 2024
// Main file for Audio Visualizer app

#include "audioVisualizer.hpp"


void AudioVisualizer::loadWindow(){
    window.create(sf::VideoMode(windowWidth, windowHeight), windowTitle,
    sf::Style::Titlebar | sf::Style::Close);
}

/* UploadButton AudioVisualizer::loadUploadButton(){
    UploadButton myUploadButton(sf::Vector2f(windowWidthFloat, windowHeightFloat), 
    font, uploadButtonText, sf::Vector2f(buttonWidth, buttonHeight), fontSize);
    return myUploadButton;
} */

ProgressBar AudioVisualizer::loadProgressBar(){
    ProgressBar loadingBar(loadingStages, sf::Vector2f(windowWidthFloat, windowHeightFloat),
    sf::Vector2f(buttonWidth, (buttonHeight * 0.5)));
    return loadingBar;
}
void AudioVisualizer::startAudioVisualizer(){
    loadWindow();

    // Crete instance of SceneManager
    SceneManager sceneManager;

    // Create upload music scene
    sceneManager.loadScene("upload music", window);

    //UploadButton uploadButton = loadUploadButton();
    ProgressBar progressBar = loadProgressBar();

    // Start the app loop
    while (window.isOpen()){
        ShowWindow(GetConsoleWindow(), SW_HIDE); // Hide console window

        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){ // Close window: exit
                window.close();
            }

            // Mouse click actions 
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
                sceneManager.clickActions(window);
            }

            // Mouse hover actions
            else if (event.type == sf::Event::MouseMoved){
                //uploadButton.updateColor(window); //update button color if mouse hover over button
                sceneManager.cursorActions(window);
            }
        }

        
        window.clear(sceneManager.getWindowClearColor());
        sceneManager.renderScene(window);

        // Conditional statement that controls how button is displayed on screen
        /* else if (progressBar.clearProgressBar == false){
            sf::Color backgroundColor(240, 255, 255);
            sf::Color maskedColor(0, 255, 255, 50);
            // backgroundShape Design
            progressBar.shapeDesign(backgroundColor, maskedColor);

            // setup text
            progressBar.setText(font, (fontSize - 20));

            progressBar.updateProgressBar(2, "Processing ...");
            progressBar.draw(window);
        } */
        if (sceneManager.shouldMoveToNextScene() == true){
            sceneId = sceneManager.getNextSceneId();
            sceneManager.loadNextScene(sceneId, window);
        }

        window.display(); // Update the window
    }
}

