#version 400
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
layout(location=2) in vec2 VertexTexcoord;
layout(location=3) in vec4 VertexTexcoord2;
layout(location=4) in vec4 VertexTexcoord3;

out vec3 Position;
out vec3 Normal;
out vec2 Texcoord;
out vec3 ViewDirection;
out vec3 Tangent;
out vec3 Bitangent;

uniform vec3 EyePos;
uniform mat4 ModelMat;
uniform mat4 ModelViewProjMat;

void main()
{

// Toon

	Position = (ModelMat * VertexPos).xyz;
	Normal = normalize(ModelMat * VertexNormal).xyz;
	Texcoord = VertexTexcoord;
	gl_Position = ModelViewProjMat * VertexPos;

// Glossy
//    Position = (ModelViewProjMat * VertexPos).xyz;
//	Normal = (ModelMat * vec4(VertexNormal.xyz,0)).xyz;
//	Texcoord = VertexTexcoord;
//	ViewDirection = -ModelViewProjMat[3].xyz;
//	gl_Position = ModelViewProjMat * VertexPos;

// Phong
//	Position = (ModelMat * VertexPos).xyz;
//    Normal = (ModelMat * vec4(VertexNormal.xyz,0)).xyz;
//    Texcoord = VertexTexcoord;
//
//    Tangent = (ModelMat * vec4(VertexTexcoord2.xyz,0)).xyz;
//    Bitangent = (ModelMat * vec4(VertexTexcoord3.xyz,0)).xyz;
//
//    gl_Position = ModelViewProjMat * VertexPos;

// RIP
//  Position = (ModelMat * VertexPos).xyz;
//  mat3 normalMatrix = transpose(inverse(mat3(ModelMat)));
//  Normal = normalize(normalMatrix * VertexNormal);
//  Texcoord = VertexTexcoord;
//  gl_Position = ModelViewProjMat * VertexPos;


}




