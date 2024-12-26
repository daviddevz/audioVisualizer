// Written by David I. 2024
// Return instances of scenes

#pragma once
#include <string>
#include "uploadMusic.hpp"
#include "processAudio.hpp"
#include "visualization.hpp"

class SceneRegistry{
private:
public:
    SceneRegistry(){};

    
    static Scene* getScene(const std::string& sceneId){
        if (sceneId == "upload music"){
            UploadMusic* uploadMusic = new UploadMusic();
            return uploadMusic;
        }
        /* else if (sceneId == "process audio"){
            ProcessAudio* processAudio = new ProcessAudio();
            return processAudio;
        } */
        else if (sceneId == "visualization"){
            Visualization* visualization = new Visualization();
            return visualization;
        }
        return nullptr;
    }

    ~SceneRegistry(){};    
};