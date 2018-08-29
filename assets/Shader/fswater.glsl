#version 400

in vec3 Position;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 LightPos;
uniform vec3 LightColor;

uniform vec3 EyePos;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;
uniform sampler2D DiffuseTexture;


void main()
{
    //FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}
