#version 400
in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;

uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform float SpecularExp;
uniform vec3 AmbientColor;
uniform sampler2D DiffuseTexture;

uniform float Time;
uniform int Type;
uniform float Speed;

uniform vec3 LightPos;
uniform vec3 LightColor;
uniform vec3 EyePos;

uniform mat4 ModelMat;
uniform mat4 ModelViewProjMat;

const int MAX_LIGHTS=14;
struct Light
{
	int Type;
	vec3 Color;
	vec3 Position;
	vec3 Direction;
	vec3 Attenuation;
	vec3 SpotRadius;
	int ShadowIndex;
};

uniform Lights 
{
	int LightCount;
	Light lights[MAX_LIGHTS];
};


const int levels = 6;
const float scaleFactor = 1.5 / levels;

vec2 getTexturePosition(vec2 texcoord) {
	vec2 direction = vec2(0.0, -1.0);
	return texcoord + (Time * Speed * 0.42 * direction);
}

void main()
{
	vec2 texcoord = Texcoord;
	if(Type == 1){
		texcoord = getTexturePosition(texcoord);
	} 

	vec4 diffuseTexture = texture(DiffuseTexture, Texcoord);

	vec3 N = normalize(Normal);
	vec3 L = normalize(LightPos - Position);
	vec3 E = normalize(EyePos - Position);
 	vec3 H = normalize(E + L);
	
    vec3 diffuseComponent = vec3(0,0,0);
    vec3 specularComponent = vec3(0,0,0);
	vec3 color = vec3(0,0,0);

//	if(lights[0].Type == 1)
//	{
//		color = lights[0].Color;
//		diffuseComponent = color  * sat(dot(N,L));
//		specularComponent = color * pow(sat(dot(N,H)), SpecularExp);
//	}

	float diffuse = max(0, dot(L,N));
    diffuseComponent = LightColor * DiffuseColor * floor(diffuse * levels) * scaleFactor ;
	
	float specular = 0.0;
	if( dot(L,N) > 0.0)
	{
		specular = pow( max(0, dot(H, N)), SpecularExp);
	}	
    specularComponent = LightColor * SpecularColor * specular;
    
	float specMask = (pow(dot(H, N), SpecularExp) > 0.4) ? 1 : 0;
	float edgeDetection = (dot(E, N) > 0.25) ? 1 : 0;
 
    gl_FragColor =  vec4(edgeDetection * diffuseComponent  * diffuseTexture.rgb + specularComponent * specMask, 1);
}




