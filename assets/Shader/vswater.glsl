#version 400
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;

out vec3 Position;
out vec3 Normal;

uniform mat4 ModelMat;
uniform mat4 ModelViewProjMat;

uniform float Steepness;

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
    if(Steepness == 0.0){
        //Position = (ModelMat * (VertexPos * mat4(10)).xyz;
        //Steepness = 1.0;

    }else {
        //Position = (ModelMat * VertexPos / mat4(10)).xyz;
        //Steepness = 0.0;
    }

    //mat4 Pos = VertexPos * mat4(100);
    Position = (ModelMat * (VertexPos * mat4(100))).xyz;
    //Position = (ModelMat * VertexPos ).xyz;
    
    Normal = (ModelMat * vec4(VertexNormal.xyz,0)).xyz;

    gl_Position = ModelViewProjMat * VertexPos;
}

