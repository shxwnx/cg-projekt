#version 400
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
layout(location=2) in vec4 VertexTexcoord;
layout(location=3) in vec4 VertexTexcoord2;
layout(location=4) in vec4 VertexTexcoord3;

out vec3 Position;
out vec3 Normal;
out vec4 Texcoord;
out vec3 ViewDirection;
out vec3 Tangent;
out vec3 Bitangent;

uniform mat4 ModelMat;
uniform mat4 ModelViewProjMat;

void main()
{

//	vec4 v_coord4 = vec4(v_coord, 1.0);
//  mat4 mvp = p*v*m;
//  position = m * v_coord4;
//  varyingNormalDirection = normalize(m_3x3_inv_transp * v_normal);
//
//  texCoords = v_coord4;
//  gl_Position = mvp * v_coord4;

//Glossy
//    Position = (ModelViewProjMat * VertexPos).xyz;
//	Normal = (ModelMat * vec4(VertexNormal.xyz,0)).xyz;
//	Texcoord = VertexTexcoord;
//	ViewDirection = -ModelViewProjMat[3].xyz;
//	gl_Position = ModelViewProjMat * VertexPos;

//Phong
	Position = (ModelMat * VertexPos).xyz;
    Normal = (ModelMat * vec4(VertexNormal.xyz,0)).xyz;
    Texcoord = VertexTexcoord;

    Tangent = (ModelMat * vec4(VertexTexcoord2.xyz,0)).xyz;
    Bitangent = (ModelMat * vec4(VertexTexcoord3.xyz,0)).xyz;

    gl_Position = ModelViewProjMat * VertexPos;
}



