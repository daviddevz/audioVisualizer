#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "sceneRegistry.hpp"

class SceneManager{
    private:
        Scene* currentScene; // Will be used for runtime polymorphism Base ptr = New Derived
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
                if (sceneId == sceneIds[2]){ 
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
                setScene(sceneId);
                
                loadScene(sceneId, window);
            }

            // If no music file is selected, go back to upload scene
            if (filePath.empty()){
                unloadScenes();
                setScene(sceneIds[0]);
                loadScene(sceneIds[0], window);
            }
        }

        bool shouldMoveToNextScene(){
            if (currentScene != nullptr && currentScene -> shouldMoveToNextScene()){
                return true;
            }
            return false;
        }

        std::string getNextSceneId(){
            if (currentScene != nullptr){
                sceneIdx += 1;
                return sceneIds[sceneIdx];
            }
            return "";
        }

        void unloadScenes(){
            delete currentScene;
        }

        ~SceneManager(){
            delete currentScene;
            delete sceneRegistry;
        };
};