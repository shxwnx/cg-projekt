#version 400
out vec4 FragColor;

uniform vec3 Color;

void main(void)
{
	FragColor = vec4(Color,1); 
}