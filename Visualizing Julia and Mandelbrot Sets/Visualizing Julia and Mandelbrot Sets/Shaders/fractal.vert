#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 TCS_ModelPos;
out vec3 TCS_Normal;
out vec2 TCS_TexCoords;

void main()
{
    TCS_ModelPos = aPos;
	TCS_Normal = aNormal;
    TCS_TexCoords = aTexCoord;
}