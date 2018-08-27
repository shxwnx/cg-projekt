#version 400
layout(location=0) in vec4 VertexPos;
out vec4 Position;


void main()
{
    Position = VertexPos;
}

