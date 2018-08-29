#version 400
in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;
in vec3 ViewDirection;

uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform float SpecularExp;
uniform vec3 AmbientColor;
uniform sampler2D DiffuseTexture;

uniform vec3 LightPos;
uniform vec3 LightColor;
uniform vec3 EyePos;


float sat( in float a)
{
    return clamp(a, 0.0, 1.0);
}

void main()
{
	vec4 diffuseTexture = texture( DiffuseTexture, Texcoord);

	vec3 normal = normalize(Normal);
	vec3 viewDirection = normalize(ViewDirection);
	vec3 reflection = normalize(reflect(viewDirection, normal)); 

	float inverseDotView = 1.0 - max(dot(normal, viewDirection), 0.0);

//	float specularComponent = pow(SpecularColor, SpecularExp);
	vec3 color = (DiffuseColor + AmbientColor) *  diffuseTexture.rgb + SpecularColor;
	gl_FragColor = vec4(color, diffuseTexture.a);

}




