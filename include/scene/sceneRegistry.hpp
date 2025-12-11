#pragma once
#include <string>
#include "uploadMusic.hpp"
#include "selectVisual.hpp"
#include "visualization.hpp"

class SceneRegistry{
    private:
        const std::vector<std::string> sceneIds = {"uploadMusic", "selectVisual", "visualization"};
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
                SelectVisual* selectVisual = new SelectVisual();
                return selectVisual;
            }
            
            else if (sceneId == sceneIds[2]){
                Visualization* visualization = new Visualization();
                return visualization;
            }
            return nullptr;
        }

        ~SceneRegistry() = default;    
};