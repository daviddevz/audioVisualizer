#pragma once
#include <string>

//Derived Scene classes - don't need to include scene header file
#include "uploadMusic.hpp"
#include "visualization.hpp"

class SceneRegistry{
private:
    const std::vector<std::string> sceneIds = {"upload music", "visualization"};
public:
    SceneRegistry() = default;
    
    const std::vector<std::string> getSceneIds(){
        return sceneIds;
    }

    Scene* getScene(const std::string& sceneId){
        if (sceneId == sceneIds[0]){
            UploadMusic* uploadMusic = new UploadMusic();
            return uploadMusic;
        }
        else if (sceneId == sceneIds[1]){
            Visualization* visualization = new Visualization();
            return visualization;
        }
        return nullptr;
    }

    ~SceneRegistry() = default;    
};