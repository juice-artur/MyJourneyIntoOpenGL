cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(PROJECT_NAME OpenGLJourney)
project(${PROJECT_NAME})

file(GLOB_RECURSE SHADERS "resources/shader/*.*")
source_group("Shaders" FILES ${SHADERS})

add_executable(${PROJECT_NAME} src/main.cpp src/shader.h src/stb_image.h src/stb_image.cpp ${SHADERS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} glm)


set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

