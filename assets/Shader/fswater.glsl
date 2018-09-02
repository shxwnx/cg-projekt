#version 410

in vec3 Position;
in vec3 Normal;
in vec4 Texcoord;

uniform mat4 ModelMat;
uniform mat4 ModelViewProjMat;
uniform float Time;
uniform float Speed;
uniform vec2 WaterSize;

uniform float SpeedValues[5];
uniform vec3 DirectionValues[5];

uniform vec3 LightPos;
uniform vec3 LightColor;
uniform vec3 EyePos;
uniform sampler2D SurfaceTexture;

vec2 getTexturePosition(vec2 texcoord) {
	vec2 direction = vec2(DirectionValues[0].x, DirectionValues[0].z);
	return texcoord + (Time * Speed * 0.42 * direction);
}

float sat(in float a)
{
	return clamp(a, 0.0, 1.0);
}

const int levels = 6;
const float scaleFactor = 1.5 / levels;
const float shininess = 0.1f;

void main()
{  
	vec2 texcoord;
	texcoord.x = Texcoord.x * (WaterSize.x / 5);
	texcoord.y = Texcoord.y * (WaterSize.y / 5);
	texcoord = getTexturePosition(texcoord);

	vec4 surface = texture(SurfaceTexture, texcoord);

	vec4 color = vec4(0.1, 0.8, 1.0, 0.7) + surface.r * 0.05;

	vec3 N = normalize(Normal);
	vec3 L = normalize(LightPos - Position);
	vec3 E = normalize(EyePos - Position);
	vec3 H = normalize(E + L);

	vec3 diffuseComponent = vec3(0, 0, 0);
	vec3 specularComponent = vec3(0, 0, 0);


	float diffuse = max(0, dot(L, N));
	diffuseComponent = LightColor * color.xyz * floor(diffuse * levels) * scaleFactor;

	float specular = 0.0;
	if (dot(L, N) > 0.0)
	{
		specular = pow(max(0, dot(H, N)), shininess);
	}
	specularComponent = LightColor * color.xyz * specular;

	float specMask = (pow(dot(H, N), shininess) > 0.4) ? 1 : 0;
	float edgeDetection = (dot(E, N) > 0.02) ? 1 : 0;

    gl_FragColor = vec4(edgeDetection * diffuseComponent  * color.xyz * specularComponent * specMask,color.a);
}
