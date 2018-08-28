#version 400
in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;

out vec4 FragColor;

uniform vec3 LightPos;
uniform vec3 LightColor;

uniform vec3 EyePos;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;
uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;

float sat( in float a)
{
    return clamp(a, 0.0, 1.0);
}

void main()
{
	vec4 DiffTex = texture( DiffuseTexture, Texcoord);
    FragColor =  vec4((DiffuseColor + AmbientColor)*DiffTex.rgb + SpecularColor , DiffTex.a);
}




