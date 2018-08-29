#include "DuckShader.h"

#define SHADER_DIRECTORY "../../assets/Shader/"

DuckShader::DuckShader() :
	diffuseColor(0.8f, 0.8f, 0.8f),
	specularColor(0.5f, 0.5f, 0.5f),
	ambientColor(0.2f, 0.2f, 0.2f),
	specularExp(20.0f),
	lightPos(20.0f, 20.0f, 20.0f),
	lightColor(1, 1, 1),
	diffuseTexture(Texture::defaultTex())
{
	//this->color = color;
	bool loaded = load(SHADER_DIRECTORY "vsduck.glsl", SHADER_DIRECTORY "fsduck.glsl");
	if (!loaded)
		throw std::exception();

	this->modelMatLoc = getParameterID("ModelMat");
	this->modelViewProjLoc = getParameterID("ModelViewProjMat");
	this->diffuseColorLoc = getParameterID("DiffuseColor");
	this->specularColorLoc = getParameterID("SpecularColor");
	this->specularExpLoc = getParameterID("SpecularExp");
	this->ambientColorLoc = getParameterID("AmbientColor");
	this->lightPosLoc = getParameterID("LightPos");
	this->lightColorLoc = getParameterID("LightColor");
	this->eyePosLoc = getParameterID("EyePos");
	this->diffuseTexLoc = getParameterID("DiffuseTexture");
}

void DuckShader::activate(const BaseCamera& camera)	 const
{
	BaseShader::activate(camera);

	setParameter(this->diffuseColorLoc, this->diffuseColor);
	setParameter(this->ambientColorLoc, this->ambientColor);
	setParameter(this->specularColorLoc, this->specularColor);
	setParameter(this->specularExpLoc, this->specularExp);

	this->diffuseTexture->activate(0);
	setParameter(this->diffuseTexLoc, 0);

	setParameter(this->lightPosLoc, this->lightPos);
	setParameter(this->lightColorLoc, this->lightColor);

	Matrix modelViewProj = camera.getProjectionMatrix() * camera.getViewMatrix() * modelTransform();
	setParameter(this->modelMatLoc, modelTransform());
	setParameter(this->modelViewProjLoc, modelViewProj);

	setParameter(eyePosLoc, camera.position());
}
