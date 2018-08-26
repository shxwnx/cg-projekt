#version 400
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;

out vec3 Position;
out vec3 Normal;

uniform mat4 ModelMat;
uniform mat4 ModelViewProjMat;

const int MAX_WAVES=5;
struct Wave
{
	float Steepness;
	float Wavelength;
	float Amplitude;
	float Speed;
    vec2 Direction;
    float Frequency;
    float Phase;
};

uniform Waves 
{
	int WaveCount;
	Wave waves[MAX_WAVES];
};

void main()
{

    //hier Position und Normal berechnen


    Position = (ModelMat * VertexPos).xyz;
    Normal = (ModelMat * vec4(VertexNormal.xyz,0)).xyz;

    gl_Position = ModelViewProjMat * VertexPos;
}

