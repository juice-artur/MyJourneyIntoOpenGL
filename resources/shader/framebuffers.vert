#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 Normal;
out vec3 Position;


layout(std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}