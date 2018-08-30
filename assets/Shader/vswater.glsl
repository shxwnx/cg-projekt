#version 410
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;

out vec3 Position;
out vec3 Normal;

out vec3 Bitangent;
out vec3 Tangent;


uniform mat4 ModelMat;
uniform mat4 ModelViewProjMat;

const float pi = 3.14159;

uniform float Time;
//uniform float Steepness;
uniform int numWaves;
uniform float WavelengthValues[5];
uniform float AmplitudeValues[5];
uniform float SpeedValues[5];
uniform vec3 DirectionValues[5];

float positionY(int i, float x, float z) {
    float frequency = 2*pi/WavelengthValues[i];
    float phase = SpeedValues[i] * frequency;
    float theta = dot(DirectionValues[i].xz, vec2(x, z));
    return AmplitudeValues[i] * sin(theta * frequency + Time * phase);
}

float normalX(int i, float x, float z) {
    float frequency = 2*pi/WavelengthValues[i];
    float phase = SpeedValues[i] * frequency;
    float theta = dot(DirectionValues[i].xz, vec2(x, z));
    float A = AmplitudeValues[i] * DirectionValues[i].x * frequency;
    return A * cos(theta * frequency + Time * phase);
}

float normalZ(int i, float x, float z) {
    float frequency = 2*pi/WavelengthValues[i];
    float phase = SpeedValues[i] * frequency;
    float theta = dot(DirectionValues[i].xz, vec2(x, z));
    float A = AmplitudeValues[i] * DirectionValues[i].z * frequency;
    return A * cos(theta * frequency + Time * phase);
}

vec3 waveNormal(float x, float z) {
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

	float x = VertexPos.x;
	float z = VertexPos.z;
	float y = 0.0;
	for (int i = 0; i < numWaves; ++i) {
		y += positionY(i, x, z);
	}
	
	Position =  (ModelMat * vec4(x, y ,z, 0)).xyz;
	
	vec4 pos = VertexPos;
    pos.y = y;
    vec3 worldNormal = waveNormal(pos.x, pos.z);
    Normal = VertexNormal.xyz * worldNormal;

    gl_Position = ModelViewProjMat * pos;

	//Position
	//vec2 horizontalPosition = VertexPos.xz;
	//vec2 horizontalDirection = Direction.xz;

	//float trigoValue = dot(horizontalDirection * Frequency, horizontalPosition) + (Phase * Time);

	//float x = VertexPos.x + ((Steepness * Amplitude) * Direction.x * cos(trigoValue));
	//float z = VertexPos.z + ((Steepness * Amplitude) * Direction.z * cos(trigoValue));

	//float y = Amplitude * sin(trigoValue);

	//Position =  (ModelMat * vec4(x, y, z, 0)).xyz;
	//Position = (ModelMat * VertexPos).xyz;///////////////////

	//Normal
	//float wa = Frequency * Amplitude;
	//float trigoValueNormal = dot(Direction * Frequency, VertexPos.xyz) + (Phase * Time);
	//float s = sin(trigoValueNormal);
	//float c = cos (trigoValueNormal);

	//float xNormal = -(Direction.x * wa * c);
	//float yNormal = 1 - (Steepness * wa * s);
	//float zNormal = -(Direction.z * wa * c);

	//Normal = (ModelMat * vec4(xNormal, yNormal, zNormal, 0)).xyz;
	//Normal = (ModelMat * vec4(VertexNormal.xyz,0)).xyz;/////////////////

    //gl_Position = ModelViewProjMat * vec4(x, y, z, 0);
	//gl_Normal = ModelViewProjMat * vec4(xNormal, yNormal, zNormal, 0);//////////////////
	//gl_Position = ModelViewProjMat * VertexPos;/////////////////////
}

