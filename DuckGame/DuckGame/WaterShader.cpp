#include "WaterShader.h"
#include <string>

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/Shader/"
#else
#define ASSET_DIRECTORY "../assets/Shader/"
#endif

WaterShader::WaterShader()
{
	bool loaded = load(ASSET_DIRECTORY"vswater.glsl", ASSET_DIRECTORY"fswater.glsl");
	if (!loaded)
		throw std::exception();
	assignLocations();
}

WaterShader::~WaterShader()
{

}

void WaterShader::activate(const BaseCamera& Cam) const {
	BaseShader::activate(Cam);

	//always update
	Matrix ModelViewProj = Cam.getProjectionMatrix() * Cam.getViewMatrix() * modelTransform();
	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, modelTransform().m);
	glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);
}

void WaterShader::assignLocations() {
	DiffuseColorLoc = glGetUniformLocation(ShaderProgram, "DiffuseColor");
	AmbientColorLoc = glGetUniformLocation(ShaderProgram, "AmbientColor");
	SpecularColorLoc = glGetUniformLocation(ShaderProgram, "SpecularColor");
	SpecularExpLoc = glGetUniformLocation(ShaderProgram, "SpecularExp");
	DiffuseTexLoc = glGetUniformLocation(ShaderProgram, "DiffuseTexture");
	LightPosLoc = glGetUniformLocation(ShaderProgram, "LightPos");
	LightColorLoc = glGetUniformLocation(ShaderProgram, "LightColor");
	EyePosLoc = glGetUniformLocation(ShaderProgram, "EyePos");

	ModelMatLoc = glGetUniformLocation(ShaderProgram, "ModelMat");
	ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
}