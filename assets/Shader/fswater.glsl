#version 410

in vec3 Position;
in vec3 Normal;
in vec4 Texcoord;

uniform float SpeedValues[5];
uniform vec3 DirectionValues[5];

uniform vec3 LightPos;
uniform vec3 LightColor;

uniform vec3 EyePos;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;

uniform vec4 Camera;
uniform sampler2D DepthMap;
uniform vec4 ScreenSize;
uniform mat4 ModelMat;
uniform mat4 ModelViewProjMat;
uniform float Time;

uniform sampler2D SurfaceTexture;
uniform vec2 WaterSize;

//uniform sampler2D reflectionTexture;
//uniform sampler2D refractionTexture;

float linearizeDepth(float z) {
        float tmp = z * 2.0 - 1.0;
        return 1.0 / (Camera.z * tmp + Camera.w);
}

float unpackFloat(vec4 rgbaDepth) {
        const vec4 bitShift = vec4(1.0 / (256.0 * 256.0 * 256.0), 1.0 / (256.0 * 256.0), 1.0 / 256.0, 1.0);
        return dot(rgbaDepth, bitShift);
}

float getLinearScreenDepth(vec2 uv) {
        return linearizeDepth(texture2D(DepthMap, uv).r) * Camera.y;
}

float getLinearDepth(vec3 pos) {
    return -(ModelViewProjMat * vec4(pos, 1.0)).z;
}

float getLinearScreenDepth() {
    vec2 uv = gl_FragCoord.xy * ScreenSize.zw;
    return getLinearScreenDepth(uv);
}

vec2 getTexturePosition(vec2 texcoord) {
	float speed = SpeedValues[0];
	vec2 direction = vec2(DirectionValues[0].x, DirectionValues[0].z);

	return texcoord + (Time * speed * 0.1 * direction);
}


void main()
{
//	  vec4 color = vec4(0.0,0.7,1.0,0.5);
//    
//    vec2 pos = position.xz;
//    pos.y += time * 0.02;
//    
//    vec4 waterlines = texture2d(surfacetexture,pos);
//    color.rgba += waterlines.r * 0.1;

	
	
//    float worlddepth = getLinearDepth(Position);
//    float screendepth = getLinearScreenDepth();
//    float foamline = clamp((screendepth - worlddepth),0.0,1.0) ;
//    if(foamline < 0.7){
//        color.rgba += 0.2;
//    }
  
	vec2 texcoord;
	texcoord.x = Texcoord.x * (WaterSize.x / 5);
	texcoord.y = Texcoord.y * (WaterSize.y / 5);
	texcoord = getTexturePosition(texcoord);

	vec4 surface = texture(SurfaceTexture, texcoord);

	vec4 color = vec4(0.0,0.7,1.0,0.5) + surface.r * 0.1; 
	vec4 color2 = vec4(0.5,0.9,1.0,0.5) + surface.r * 0.1;
	
	color = mix(color, color2, Position.y);

	gl_FragColor = color;
    //gl_FragColor = vec4(0.0,0.7,1.0,0.3);
}
