#version 430 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 InTex;

uniform mat4 gVP;
uniform mat4 modelMatrix;

out vec3 Color;
out vec3 worldPosition;
out vec3 Normal;
out vec2 Tex;
out vec3 localPosition;

void main()
{
    gl_Position = gVP * vec4(vertexPosition, 1.0);
    
    Color = vec3(1.0f, 0.0f, 0.0f);

    worldPosition = (modelMatrix * vec4(vertexPosition,1)).xyz;

    Normal = (modelMatrix * vec4(vertexNormal,0)).xyz;
    
    Tex = InTex;
    
    localPosition = vertexPosition;
}