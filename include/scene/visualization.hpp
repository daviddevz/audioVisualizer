// Written by David I. 2024
// Scene to visualize music

#pragma once
#include "SFML/Window.hpp"
#include "scene/scene.hpp"

class Vizualization : public Scene{
public:
    Vizualization(){};
    UploadButton loadUploadButton();
    ~Vizualization(){};

private:
};