#pragma once

#include "../engine/Scene.h"
#include "../engine/Assets.h"

#include "CubeObject.h"

class CubeMesh;

class Scene_Cyril_Tesselation : public Scene
{
public:
    Scene_Cyril_Tesselation();
    ~Scene_Cyril_Tesselation();
    void load();
    void clean();
    void pause();
    void resume();
    void handleEvent(const InputState&);
    void update(float dt);
    void draw();
    void setGame(Game*);

private:
    Game* game;
    CubeMesh* cubeMesh;

    Matrix4 projection;
    Shader shader;

    vector<CubeObject> cubes;
    float newXPosition{ 0 };
};

