#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "sceneRegistry.hpp"

class SceneManager{
private:
    Scene* currentScene; // Creating a pointer to scene class that has derived scene classes
    SceneRegistry* sceneRegistry = new SceneRegistry();

    std::vector<std::string> sceneIds = sceneRegistry -> getSceneIds();

    std::string lastSceneId, filePath;
    int sceneIdx = 0;

public:
    SceneManager(sf::RenderWindow& window){
        setScene(sceneIds[0]);
        loadScene(sceneIds[0], window);
    };

    void setScene(std::string sceneId){
        currentScene = sceneRegistry -> getScene(sceneId);
    }

    void loadScene(const std::string& sceneId, sf::RenderWindow& window){
        lastSceneId = sceneId;
        if (currentScene == nullptr){
            throw std::runtime_error("Scene not found");
        }
        else{
            if (sceneId == sceneIds[1]){ 
                // visualization id
                currentScene -> setFilePath(filePath);
            }

            currentScene -> load(window);
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
        if (lastSceneId == sceneIds[0]){ 
            filePath = currentScene -> getFilePath();
        }

        if (sceneId.empty()){
            unloadScenes();
            throw std::runtime_error("Found no next scene to load");
        }
        else{
            unloadScenes();
            currentScene = sceneRegistry -> getScene(sceneId);
            loadScene(sceneId, window);
        }

        // If no music file is selected, go back to upload scene
        if (filePath.empty()){
            unloadScenes();
            currentScene = sceneRegistry -> getScene(sceneIds[0]);
            loadScene(sceneIds[0], window);
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
            //return currentScene -> getNextSceneId(sceneIds, lastSceneId);
            //std::vector<std::string>::iterator it = std::find(sceneIds.begin(), sceneIds.end(), lastSceneId);

            /* if (it != sceneIds.end() && std::next(it) != sceneIds.end()){
                it += 1;
                return *it;
            } */
           sceneIdx += 1;
           //std::cout<<"SceneId: "<<sceneIdx <<std::endl;
           return sceneIds[sceneIdx];
        }
        return "";
    }

    void unloadScenes(){
        delete currentScene;
        delete sceneRegistry;
    }

    ~SceneManager(){
        unloadScenes();
    };
};