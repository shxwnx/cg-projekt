#version 400
out vec4 FragColor;
in vec4 Position;

uniform float MinDistance;
uniform float MaxDistance;
uniform vec3 CameraPos;

void main(void)
{
    float Distance = length(Position.xyz-CameraPos);
    float v = clamp((Distance-MinDistance) / (MaxDistance-MinDistance), 0, 1);
	FragColor = vec4(vec3(abs(v)), 1);
}