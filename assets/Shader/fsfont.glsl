#version 400
in vec2 TexCoords;

uniform sampler2D text;
uniform vec3 Color;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    gl_FragColor = vec4(Color, 1.0) * sampled;
}  