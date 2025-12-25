#include "audioVisualizer.hpp"

void AudioVisualizer::loadWindow(){
    window.create(sf::VideoMode(windowWidth, windowHeight), windowTitle,
    sf::Style::Titlebar | sf::Style::Close);
}

void AudioVisualizer::startAudioVisualizer(){
    loadWindow();
    SceneManager sceneManager(window);
    window.setFramerateLimit(fps);
    
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }

            if (event.type == sf::Event::MouseButtonReleased &&
                event.mouseButton.button == sf::Mouse::Left){
                sceneManager.clickActions(window);
            }

            if (event.type == sf::Event::MouseMoved){
                sceneManager.cursorActions(window, window);
            }
        }
        
        window.clear();

        if (sceneManager.shouldMoveToNextScene()){
            sceneId = sceneManager.getNextSceneId();
            sceneManager.loadNextScene(sceneId, window);
        }
        sceneManager.renderScene(window);
        
        window.display();
    }
}
