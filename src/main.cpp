#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "camera.h"
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "model.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int LoadTexture(char const* path);
unsigned int LoadCubemap(vector<std::string> faces);

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

    float cubeVertices[] = {
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

    float skyboxVertices[] = {
        -1.0f, 1.0f, -1.0f,   //
        -1.0f, -1.0f, -1.0f,  //
        1.0f, -1.0f, -1.0f,   //
        1.0f, -1.0f, -1.0f,   //
        1.0f, 1.0f, -1.0f,    //
        -1.0f, 1.0f, -1.0f,   //
                              //
        -1.0f, -1.0f, 1.0f,   //
        -1.0f, -1.0f, -1.0f,  //
        -1.0f, 1.0f, -1.0f,   //
        -1.0f, 1.0f, -1.0f,   //
        -1.0f, 1.0f, 1.0f,    //
        -1.0f, -1.0f, 1.0f,   //
                              //
        1.0f, -1.0f, -1.0f,   //
        1.0f, -1.0f, 1.0f,    //
        1.0f, 1.0f, 1.0f,     //
        1.0f, 1.0f, 1.0f,     //
        1.0f, 1.0f, -1.0f,    //
        1.0f, -1.0f, -1.0f,   //
                              //
        -1.0f, -1.0f, 1.0f,   //
        -1.0f, 1.0f, 1.0f,    //
        1.0f, 1.0f, 1.0f,     //
        1.0f, 1.0f, 1.0f,     //
        1.0f, -1.0f, 1.0f,    //
        -1.0f, -1.0f, 1.0f,   //
                              //
        -1.0f, 1.0f, -1.0f,   //
        1.0f, 1.0f, -1.0f,    //
        1.0f, 1.0f, 1.0f,     //
        1.0f, 1.0f, 1.0f,     //
        -1.0f, 1.0f, 1.0f,    //
        -1.0f, 1.0f, -1.0f,   //
                              //
        -1.0f, -1.0f, -1.0f,  //
        -1.0f, -1.0f, 1.0f,   //
        1.0f, -1.0f, -1.0f,   //
        1.0f, -1.0f, -1.0f,   //
        -1.0f, -1.0f, 1.0f,   //
        1.0f, -1.0f, 1.0f     //
    };

    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    Shader shader("../resources/shader/framebuffers.vert", "../resources/shader/framebuffers.frag");

    Shader skyboxShader("../resources/shader/skybox.vert", "../resources/shader/skybox.frag");

    shader.Use();
    shader.SetInt("texture1", 0);

    unsigned int cubeTexture = LoadTexture("../resources/textures/wooden_container.jpg");
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    vector<std::string> faces{
        "../resources/textures/skybox/right.jpg",   //
        "../resources/textures/skybox/left.jpg",    //
        "../resources/textures/skybox/top.jpg",     //
        "../resources/textures/skybox/bottom.jpg",  //
        "../resources/textures/skybox/front.jpg",   //
        "../resources/textures/skybox/back.jpg"     //
    };
    unsigned int cubemapTexture = LoadCubemap(faces);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        ProcessInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        shader.SetMat4("model", model);
        shader.SetMat4("view", view);
        shader.SetMat4("projection", projection);

        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.SetMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
 

        glDepthFunc(GL_LEQUAL);
        skyboxShader.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyboxShader.SetMat4("view", view);
        skyboxShader.SetMat4("projection", projection);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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
unsigned int LoadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int LoadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}