#pragma once
#include <string>

/* Derived Scene classes*/
#include "uploadMusic.hpp"
#include "visualization.hpp"

class SceneRegistry{
private:
public:
    SceneRegistry() = default;
    
    static Scene* getScene(const std::string& sceneId){
        if (sceneId == "upload music"){
            UploadMusic* uploadMusic = new UploadMusic();
            return uploadMusic;
        }
        else if (sceneId == "visualization"){
            Visualization* visualization = new Visualization();
            return visualization;
        }
        return nullptr;
    }

    ~SceneRegistry() = default;    
};