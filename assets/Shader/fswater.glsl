#version 410

in vec3 Position;
in vec3 Normal;

uniform vec3 LightPos;
uniform vec3 LightColor;

uniform vec3 EyePos;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;

vec3 reflect(vec3 I, vec3 N){
	return I - 2 * dot(I, N) * N;
}


void main()
{
    gl_FragColor = vec4(0.0, 0.0, 0.5, 1.0);
}
