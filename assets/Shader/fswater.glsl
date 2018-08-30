#version 410

in vec3 Position;
in vec3 Normal;
in vec4 Texcoord;

uniform vec3 LightPos;
uniform vec3 LightColor;

uniform vec3 EyePos;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

vec3 reflect(vec3 I, vec3 N){
	return I - 2 * dot(I, N) * N;
}


void main()
{
	//vec2 longitudeLatitude = vec2((atan(Texcoord.y, Texcoord.x) / 3.1415926 + 1.0) * 0.5,(asin(Texcoord.z) / 3.1415926 + 0.5));
	//vec4 reflectColor = texture(reflectionTexture, longitudeLatitude);
	//vec4 refractColor = texture(refractionTexture, longitudeLatitude);


    gl_FragColor = vec4(0.0, 0.0, 0.5, 1.0);
}
