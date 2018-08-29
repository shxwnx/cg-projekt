#version 410
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;

out vec3 Position;
out vec3 Normal;

uniform mat4 ModelMat;
uniform mat4 ModelViewProjMat;

uniform float Time;
uniform float Steepness;
uniform float Wavelength;
uniform float Amplitude;
uniform float Speed;
uniform vec3 Direction;
uniform float Frequency;
uniform float Phase;

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
	//Position
	vec2 horizontalPosition = VertexPos.xz;
	vec2 horizontalDirection = Direction.xz;

	float trigoValue = dot(horizontalDirection * Frequency, horizontalPosition) + (Phase * Time);

	float x = VertexPos.x + ((Steepness * Amplitude) * Direction.x * cos(trigoValue));
	float z = VertexPos.z + ((Steepness * Amplitude) * Direction.z * cos(trigoValue));

	float y = Amplitude * sin(trigoValue);

	Position =  (ModelMat * vec4(x, y, z, 0)).xyz;
	//Position = (ModelMat * VertexPos).xyz;

	//Normal
	float wa = Frequency * Amplitude;
	float trigoValueNormal = dot(Direction * Frequency, VertexPos.xyz) + (Phase * Time);
	float s = sin(trigoValueNormal);
	float c = cos (trigoValueNormal);

	float xNormal = -(Direction.x * wa * c);
	float yNormal = 1 - (Steepness * wa * s);
	float zNormal = -(Direction.z * wa * c);

	Normal = (ModelMat * vec4(xNormal, yNormal, zNormal, 0)).xyz;
	//Normal = (ModelMat * vec4(VertexNormal.xyz,0)).xyz;

    gl_Position = ModelViewProjMat * vec4(x, y, z, 0);
	gl_Normal = vec4(xNormal, yNormal, zNormal, 0);
	//gl_Position = ModelViewProjMat * VertexPos;
}

