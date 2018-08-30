#version 400
in vec3 Position;
in vec3 Normal;
in vec4 Texcoord;
in vec3 ViewDirection;
in vec3 Tangent;
in vec3 Bitangent;

uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform float SpecularExp;
uniform vec3 AmbientColor;
uniform sampler2D DiffuseTexture;

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

float sat( in float a)
{
    return clamp(a, 0.0, 1.0);
}

vec4 Ambient = vec4(0.2, 0.2, 0.2, 1.0);

void main()
{

//	vec4 diffuseTexture = texture( DiffuseTexture, Texcoord);
//    if(diffuseTexture.a <0.3f) discard;
//
//    vec3 N = normalize(Normal);
//    vec3 L = normalize(LightPos-Position);	//Light
//    vec3 E = normalize(EyePos-Position);	//Eye
//    vec3 R = reflect(-L,N);					//Reflection
//    vec3 H = normalize(E + L);				//Halfvector
//    float attenuation;
//    vec3 DiffuseComponent = vec3(0,0,0);
//    vec3 SpecularComponent = vec3(0,0,0);
//    float nDotL;
//    float spotDot;
//    float distance;
//
//	vec3 color = vec3(0,0,0);
//
//    for(int i=0; i<LightCount;i++) {
//    
//        if(lights[i].Type == 0) {			//Point
//            distance = length(lights[i].Position-Position);
//            L = normalize(lights[i].Position-Position);
//            R = reflect(-L,N);
//            H = normalize(E + L);
//            attenuation = 1.0/( lights[i].Attenuation.x + lights[i].Attenuation.y * distance + lights[i].Attenuation.z * distance * distance);
//        } else if(lights[i].Type == 1) {	//Directional
//            attenuation = 1;
//            L = -lights[i].Direction;
//            R = reflect(-L,N);
//            H = normalize(E + L);
//        }else if(lights[i].Type == 2) {		//Spot

//            distance = length(lights[i].Position-Position);
//            L = normalize(lights[i].Position-Position);
//            R = reflect(-L,N);
//            H = normalize(E + L);
//            nDotL = max(dot(N,L),0.0);
//            if(nDotL > 0.0) {
//                spotDot = dot(normalize(lights[i].Direction), normalize(-L));
//                if(acos(spotDot) > lights[i].SpotRadius.y) {
//                    attenuation = 0;
//                } else if (acos(spotDot) < lights[i].SpotRadius.x) {
//                    attenuation = 1;
//                } else if( acos(spotDot) > lights[i].SpotRadius.x && spotDot < lights[i].SpotRadius.y) {
//                    float a = acos(spotDot) - lights[i].SpotRadius.x;
//                    float b = lights[i].SpotRadius.y - lights[i].SpotRadius.x;
//                    attenuation = 1 - (a/b);
//                }
//            }
//        }
//		color = lights[i].Color * attenuation;
//        
//        DiffuseComponent += color  * sat(dot(N,L));
//        SpecularComponent += color * pow(sat(dot(N,H)), SpecularExp);
//    }
//      
//	DiffuseComponent += color  * sat(dot(N,L));
//    SpecularComponent += color * pow(sat(dot(N,H)), SpecularExp);
//    
//    DiffuseComponent *= DiffuseColor;
//    SpecularComponent *= SpecularColor;
//    
//    gl_FragColor =  vec4((DiffuseComponent + AmbientColor) * diffuseTexture.rgb + SpecularComponent, diffuseTexture.a);

	vec3 normalDirection = normalize(Normal);
	vec3 viewDirection = normalize(vec3(ModelViewProjMat * vec4(0.0, 0.0, 0.0, 1.0) - vec4(Position, 1)));
	vec3 lightDirection;
	float attenuation;

	vec2 longitudeLatitude = vec2((atan(Texcoord.y, Texcoord.x) / 3.1415926 + 1.0) * 0.5,(asin(Texcoord.z) / 3.1415926 + 0.5));
	vec4 textureColor = texture2D(DiffuseTexture, longitudeLatitude);

	vec3 ambientLighting;
	vec3 diffuseReflection;
	vec3 specularReflection;

	for(int i=0; i<LightCount;i++) 
	{     
		if(lights[i].Type == 0) //Point
		{			
			vec3 positionToLightSource = vec3(lights[i].Position - Position);
			float distance = length(positionToLightSource);
			lightDirection = normalize(positionToLightSource);
			attenuation = 1.0/( lights[i].Attenuation.x + lights[i].Attenuation.y * distance + lights[i].Attenuation.z * distance * distance);

		} 
		else if(lights[i].Type == 1) //Directional
		{	
			attenuation = 1.0; // no attenuation
			lightDirection = normalize(vec3(lights[i].Position));
		}
		else if(lights[i].Type == 2) //Spot
		{		
			vec3 positionToLightSource = vec3(lights[i].Position - Position);
			float distance = length(positionToLightSource);
			lightDirection = normalize(positionToLightSource);
			attenuation = 1.0/( lights[i].Attenuation.x + lights[i].Attenuation.y * distance + lights[i].Attenuation.z * distance * distance);
		}

		ambientLighting *= vec3(Ambient) * vec3(textureColor);
		diffuseReflection *= attenuation * DiffuseColor * lights[i].Color * vec3(textureColor) * max(0.0, dot(normalDirection, lightDirection));
		if (dot(normalDirection, lightDirection) < 0.0) // light source on the wrong side?
		{
			specularReflection *= vec3(0.0, 0.0, 0.0); // no specular reflection
		}
		else // light source on the right side
		{
			specularReflection *= attenuation * SpecularColor * lights[i].Color * (1.0 - textureColor.a) * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), 0.1f);
		}
	}
	gl_FragColor = vec4(ambientLighting + diffuseReflection + specularReflection, 1.0);
}




