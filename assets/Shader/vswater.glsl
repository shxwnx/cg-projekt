#version 410
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
layout(location=2) in vec4 VertexTexcoord;

out vec3 Position;
out vec3 Normal;
out vec4 Texcoord;

uniform mat4 ModelMat;
uniform mat4 ModelViewProjMat;

const float pi = 3.14159265358979323846;

uniform float Time;
uniform int numWaves;
uniform float WavelengthValues[5];
uniform float AmplitudeValues[5];
uniform float SpeedValues[5];
uniform vec3 DirectionValues[5];

float positionY(int i, float x, float z) 
{
    float frequency = 2*pi/WavelengthValues[i];
    float phase = SpeedValues[i] * frequency;
    float theta = dot(DirectionValues[i].xz, vec2(x, z));
    return AmplitudeValues[i] * sin(theta * frequency + Time * phase);
}

float normalX(int i, float x, float z) 
{
    float frequency = 2*pi/WavelengthValues[i];
    float phase = SpeedValues[i] * frequency;
    float theta = dot(DirectionValues[i].xz, vec2(x, z));
    float A = AmplitudeValues[i] * DirectionValues[i].x * frequency;
    return A * cos(theta * frequency + Time * phase);
}

float normalZ(int i, float x, float z) 
{
    float frequency = 2*pi/WavelengthValues[i];
    float phase = SpeedValues[i] * frequency;
    float theta = dot(DirectionValues[i].xz, vec2(x, z));
    float A = AmplitudeValues[i] * DirectionValues[i].z * frequency;
    return A * cos(theta * frequency + Time * phase);
}

vec3 waveNormal(float x, float z) 
{
	float dx = 0.0;
    float dz = 0.0;
    for (int i = 0; i < numWaves; ++i) {
        dx += normalX(i, x, z);
        dz += normalZ(i, x, z);
    }
    vec3 n = vec3(-dx, 1.0, -dz);
    return normalize(n);
}

void main()
{
	Texcoord = VertexTexcoord;

	float x = VertexPos.x;
	float z = VertexPos.z;
	float y = 0.0;
	for (int i = 0; i < numWaves; ++i) {
		y += positionY(i, x, z);
	}

	vec4 pos = VertexPos;
    pos.y = y;
	Position =  (ModelMat * pos).xyz;

    vec4 newNormal = vec4(waveNormal(pos.x, pos.z), 1.0);
	Normal = normalize(ModelMat * newNormal).xyz;

    gl_Position = ModelViewProjMat * pos;

}

