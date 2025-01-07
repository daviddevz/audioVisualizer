#include "audioVisualizer.hpp"

void AudioVisualizer::loadWindow(){
    window.create(sf::VideoMode(windowWidth, windowHeight), windowTitle,
    sf::Style::Titlebar | sf::Style::Close);
}

void AudioVisualizer::startAudioVisualizer(){
    loadWindow();

    // Crete instance of SceneManager
    SceneManager sceneManager(window);

    // Start the app loop
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }

            // Mouse click actions 
            if (event.type == sf::Event::MouseButtonReleased &&
                event.mouseButton.button == sf::Mouse::Left){
                sceneManager.clickActions(window);
            }

            // Mouse hover actions
            if (event.type == sf::Event::MouseMoved){
                sceneManager.cursorActions(window, window);
            }
        }
        
        window.clear(sceneManager.getWindowClearColor());

        if (sceneManager.shouldMoveToNextScene() == true){
            sceneId = sceneManager.getNextSceneId();
            sceneManager.loadNextScene(sceneId, window);
        }

        sceneManager.renderScene(window);
        window.display();
    }
}

