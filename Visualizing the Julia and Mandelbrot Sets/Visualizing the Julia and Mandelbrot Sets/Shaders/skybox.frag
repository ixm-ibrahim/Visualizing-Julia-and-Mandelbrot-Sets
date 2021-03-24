#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D skybox;

void main()
{
	FragColor = texture(skybox, TexCoords) * vec4(vec3(.9), 1.0);
}
