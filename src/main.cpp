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
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,   //
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,    //
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,    //
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   //
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //
                                          //
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   //
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,    //
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,     //
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,     //
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,    //
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   //
                                          //
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    //
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   //
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    //
                                          //
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,     //
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,    //
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,   //
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,   //
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,    //
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,     //
                                          //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,   //
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,    //
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,    //
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
                                          //
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   //
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,    //
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,     //
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,     //
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,    //
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f    //
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

    Shader ourShader("resources/shader/baseVertexShader.vs", "resources/shader/baseFragmentShader.fs");

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("resources/textures/wooden_container.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    ourShader.Use();
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    ourShader.SetInt("texture2", 1);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ProcessInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        int modelLoc = glGetUniformLocation(ourShader.ID, "model");

        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");

        unsigned int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        for (auto cubePosition : cubePositions)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePosition);

            float angle = 20.0f;
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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
