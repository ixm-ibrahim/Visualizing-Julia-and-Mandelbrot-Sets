#version 450 core

#define M_PI 3.1415926535897932384626433832795

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoords;
out vec2 FragPos;

uniform float zoom;
uniform vec2 radius;
uniform vec2 center;
uniform float rollAngle;


void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	TexCoords = vec2(aTexCoord.x, aTexCoord.y);


	float a = rollAngle * M_PI / 180;
	float s = sin(a);
	float c = cos(a);
	vec2 newPos = vec2(aPos.x*c - aPos.y*s, aPos.y*c + aPos.x*s);

    //FragPos = aPos.xy;
    FragPos = radius*aPos.xy/zoom;
	FragPos = center + vec2(FragPos.x*c - FragPos.y*s, FragPos.y*c + FragPos.x*s);
    //FragPos = center + radius*newPos/zoom;
}
