#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "sceneRegistry.hpp"

class SceneManager{
private:
    std::unordered_map<std::string, Scene*> scenes;
    std::vector<std::string> sceneIds = {"upload music", "visualization"};
    Scene* currentScene;
    std::string lastSceneId, filePath;

public:
    SceneManager(sf::RenderWindow& window){
        setScene();

        loadScene(sceneIds[0], window);
    };

    void setScene(){
        for (std::string sceneId : sceneIds){
            scenes[sceneId] = SceneRegistry::getScene(sceneId);
        }
    }

    void loadScene(const std::string& sceneId, sf::RenderWindow& window){
        currentScene = scenes[sceneId];
        lastSceneId = sceneId;

        if (currentScene == nullptr){
            throw std::runtime_error("Scene not found");
        }
        else{
            currentScene -> load(window);
        }

        if (sceneId == "process audio"){
            filePath = scenes["upload music"] -> getFilePath();
            scenes["process audio"] -> setFilePath(filePath);
        }
        else if (sceneId == "visualization"){
            filePath = scenes["upload music"] -> getFilePath();
            scenes["visualization"] -> setFilePath(filePath);
        }
    }

    void renderScene(sf::RenderWindow& window){
        if (currentScene != nullptr){
            currentScene -> render(window);
        }
    }

    Scene* getCurrentScene(){
        if (currentScene != nullptr){
            return currentScene;
        }
        return nullptr;
    };

    sf::Color getWindowClearColor(){
        if (currentScene != nullptr){
            return currentScene -> getClearColor();
        }
        return sf::Color::Black;
    }

    void clickActions(sf::RenderWindow& window){
        if (currentScene != nullptr){
            currentScene -> clickActions(window);
        }
    }

    void cursorActions(sf::RenderWindow& window, sf::RenderTarget& target){
        if (currentScene != nullptr){
            currentScene -> cursorActions(window, target);
        }
    }

    void loadNextScene(std::string& sceneId, sf::RenderWindow& window){
        
        // If scene is empty load the next scene
        if (sceneId.empty()){
            std::vector<std::string>::iterator it;
            it = std::find(sceneIds.begin(), sceneIds.end(), lastSceneId);

            if (it != sceneIds.end()){
                sceneId = *std::next(it, 1);
                loadScene(sceneId, window);
            }
        }
        else{
            loadScene(sceneId, window);
        }

        // If no music file is selected, go back to upload scene
        if (lastSceneId == "process audio" && filePath.empty()){
            delete scenes["upload music"];
            scenes["upload music"] = SceneRegistry::getScene("upload music");
            loadScene("upload music", window);
        }
    }

    bool shouldMoveToNextScene(){
        if (currentScene != nullptr){
            return currentScene -> shouldMoveToNextScene();
        }
        return false;
    }

    std::string getNextSceneId(){
        if (currentScene != nullptr){
            return currentScene -> getNextSceneId();
        }
        return "";
    }

    void unloadScenes(){
        for (std::string sceneId : sceneIds){
            delete scenes[sceneId];
        }
    }

    ~SceneManager(){
        delete currentScene;
        unloadScenes();
    };
};