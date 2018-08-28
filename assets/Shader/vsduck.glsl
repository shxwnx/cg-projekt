#version 400
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
layout(location=2) in vec2 VertexTexcoord;

out vec3 Position;
out vec3 Normal;
out vec2 Texcoord;
out vec3 ViewDirection;

uniform mat4 ModelMat;
uniform mat4 ModelViewProjMat;

void main()
{
    Position = (ModelViewProjMat * VertexPos).xyz;
	Normal = (ModelMat * vec4(VertexNormal.xyz,0)).xyz;
	Texcoord = VertexTexcoord;
	ViewDirectory = -ModelViewProjMat[3].xyz;

	gl_Position = ModelViewProjMat * VertexPos;
}



