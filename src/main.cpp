#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "camera.h"
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;

        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  //
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,   //
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,    //
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,    //
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,   //
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  //
                                                 //
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,    //
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,     //
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,      //
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,      //
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,     //
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,    //
                                                 //
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,    //
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,   //
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  //
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  //
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,   //
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,    //
                                                 //
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,      //
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,     //
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,    //
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,    //
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,     //
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,      //
                                                 //
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  //
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,   //
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,    //
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,    //
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,   //
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  //
                                                 //
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,    //
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,     //
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,      //
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,      //
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,     //
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f     //
    };

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),      //
        glm::vec3(2.0f, 5.0f, -15.0f),    //
        glm::vec3(-1.5f, -2.2f, -2.5f),   //
        glm::vec3(-3.8f, -2.0f, -12.3f),  //
        glm::vec3(2.4f, -0.4f, -3.5f),    //
        glm::vec3(-1.7f, 3.0f, -7.5f),    //
        glm::vec3(1.3f, -2.0f, -2.5f),    //
        glm::vec3(1.5f, 2.0f, -2.5f),     //
        glm::vec3(1.5f, 0.2f, -1.5f),     //
        glm::vec3(-1.3f, 1.0f, -1.5f)     //
    };

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Shader lightingShader("../resources/shader/object.vert", "../resources/shader/object.frag");
    Shader lampShader("../resources/shader/lamp.vert", "../resources/shader/lamp.frag");

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ProcessInput(window);

        glClearColor(0.0, 0.0, 0.0, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.Use();
        lightingShader.SetVec3("objectColor", {1.0f, 0.5f, 0.31f});
        lightingShader.SetVec3("lightColor", {1.0f, 1.0f, 1.0f});

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        lightingShader.SetMat4("projection", projection);
        lightingShader.SetMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.SetMat4("model", model);
        lightingShader.SetVec3("lightPos", lightPos);
        lightingShader.SetVec3("viewPos", camera.Position);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lampShader.Use();
        lampShader.SetMat4("projection", projection);
        lampShader.SetMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lampShader.SetMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    const float cameraSpeed = 2.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
