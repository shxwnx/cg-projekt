#version 400
layout (location = 0) in vec4 VertexPos; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 Projection;

void main()
{
	gl_Position = Projection * vec4(VertexPos.xy, 0.0, 1.0);
	TexCoords = VertexPos.zw;
}  
