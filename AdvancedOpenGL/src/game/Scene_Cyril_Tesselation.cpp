#include "Scene_Cyril_Tesselation.h"
#include "../engine/Timer.h"
#include "../engine/MacroUtils.h"

#include <cstdlib>
#include <ctime>
#include <GL/glew.h>
#include <assert.h>

#include "CubeMesh.h"

Scene_Cyril_Tesselation::Scene_Cyril_Tesselation()
{
}

Scene_Cyril_Tesselation::~Scene_Cyril_Tesselation()
{
	clean();
}

void Scene_Cyril_Tesselation::setGame(Game* _game)
{
    game = _game;
}

void Scene_Cyril_Tesselation::load()
{
    shader = Assets::loadShader(
        SHADER_VERT(SHADER_NAME),
        SHADER_FRAG(SHADER_NAME), 
        SHADER_TECS(SHADER_NAME), 
        SHADER_TESE(SHADER_NAME), 
        "",//SHADER_GEOM(SHADER_NAME), 
        SHADER_ID(SHADER_NAME)
    );
    assert(shader.isValid());

    projection = Matrix4::createPerspectiveFOV(70.0f, game->windowWidth, game->windowHeight, 0.1f, 1000.0f);

    cubeMesh = new CubeMesh();
    cubeMesh->load();

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    cubes.emplace_back(0.0f, 0.0f, cubeMesh);
}

void Scene_Cyril_Tesselation::clean()
{
	cubeMesh->clean();
	delete cubeMesh;
}

void Scene_Cyril_Tesselation::pause()
{
}

void Scene_Cyril_Tesselation::resume()
{
}

void Scene_Cyril_Tesselation::handleEvent(const InputState&)
{
}

void Scene_Cyril_Tesselation::update(float dt)
{
    for (auto& cube : cubes) 
    {
        cube.update();
    }
}

void Scene_Cyril_Tesselation::draw()
{
    static const GLfloat bgColor[] = { 0.0f, 0.0f, 0.2f, 1.0f };
    glClearBufferfv(GL_COLOR, 0, bgColor);
    shader.use();
    glPointSize(5.0f);
    shader.setMatrix4("proj_matrix", projection);

    for (auto& cube : cubes) 
    {
        cube.draw(shader);
    }
}
