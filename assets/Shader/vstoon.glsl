#version 400
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
layout(location=2) in vec2 VertexTexcoord;

out vec3 Position;
out vec3 Normal;
out vec2 Texcoord;


uniform float Time;
uniform vec3 EyePos;
uniform mat4 ModelMat;
uniform mat4 ModelViewProjMat;

const float pi = 3.14159;

uniform int Type;

float positionY() {
    float frequency = 2 * pi ;
    return  sin(frequency * Time);
}

void main()
{
	vec4 pos = vec4(0.0 ,0.0 ,0.0 ,0.0);
	if(Type == 0){
		pos = vec4(VertexPos.x, VertexPos.y + positionY(), VertexPos.z, VertexPos.w);
	} else {
		pos = VertexPos;
	}
	
	Position = (ModelMat * pos).xyz;
	Normal = normalize(ModelMat * VertexNormal).xyz;
	Texcoord = VertexTexcoord;
	gl_Position = ModelViewProjMat * pos;
}




