#include <glad/glad.h>
#include <GLFW/glfw3.h>
//------glm-------
#include <glm/glm.hpp>
#include <glm/ext.hpp>


#include <iostream>


#include "window/window.h"
#include "window/events.h"
#include "window/Camera.h"
#include "graphics/ShaderCl.h"
#include "graphics/model.h"


float vertices[] =
{
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   
     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};
int attrs[] = { 3,3,0 };

int main(void)
{
    Window::initialize(1200, 720, "Hi_animation");
    Events::initialize();
    Camera* camera = new Camera(glm::vec3(0., 5.f, 0.), radians(90.0f));

    /*Shader test_shader("../res/test_shader.vs", "../res/test_shader.fs");
    Mesh test_mesh(vertices, 3, attrs);*/
    Shader test_backpack_shader("../res/test_texture_shader.vs", "../res/test_texture_shader.fs");

    float lastFrame = static_cast<float>(glfwGetTime());
    float deltaTime;

    float camX = 0.0f;
    float camY = 0.0f;

    Model test_backpack_model("../res/jet_model/jet_model.obj");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!Window::isShouldClose())
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.5f, 0.6f, 0.6f, 1);

        int speed = 15;

        if (Events::jpressed(GLFW_KEY_ESCAPE)) {
            Window::setShouldClose(true);
        }

        if (Events::jpressed(GLFW_KEY_TAB)) {
            Events::toogleCursor();
        }
        if (Events::clicked(GLFW_MOUSE_BUTTON_1)) {
            glClearColor(0.8f, 0.4f, 0.2f, 1);
        }

        if (Events::pressed(GLFW_KEY_W)) {
            camera->position += speed * deltaTime * camera->front;
        }

        if (Events::pressed(GLFW_KEY_S)) {
            camera->position -= speed * deltaTime * camera->front;
        }

        if (Events::pressed(GLFW_KEY_A)) {
            camera->position -= speed * deltaTime * camera->right;
        }
        if (Events::pressed(GLFW_KEY_D)) {
            camera->position += speed * deltaTime * camera->right;
        }

        camera->rotate(-Events::deltaY / Window::width, -Events::deltaX / Window::height, 0);

        if (Events::_cursor_locked) {
            camY += -Events::deltaY / Window::height * 2;
            camX += -Events::deltaX / Window::height * 2;

            if (camY < -radians(89.0f)) {
                camY = -radians(89.0f);
            }
            if (camY > radians(89.0f)) {
                camY = radians(89.0f);
            }

            camera->rotation = mat4(1.0f);
            camera->rotate(camY, camX, 0);
        }
        test_backpack_shader.use();

        // view/projection transformations
        
        test_backpack_shader.setMat4("projection", camera->getProjection());
        test_backpack_shader.setMat4("view", camera->getView());

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        test_backpack_shader.setMat4("model", model);

        test_backpack_model.Draw(test_backpack_shader);

        /*glm::mat4 model = glm::mat4(1.0f);
        model = translate(model, glm::vec3(0.f, 1.f, -10.f));
        test_shader.use();
        test_shader.setMat4("projection", camera->getProjection());
        test_shader.setMat4("view", camera->getView());
        test_shader.setMat4("model", model);
        test_mesh.draw(GL_TRIANGLES);*/

        Window::swapBuffers();
        Events::pullEvents();
    }

    glfwTerminate();
    return 0;
}