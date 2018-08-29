#include "WaterShader.h"
#include <string>

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/Shader/"
#else
#define ASSET_DIRECTORY "../assets/Shader/"
#endif

WaterShader::WaterShader() :
	time(0.0f),
	steepness(0.0f),
	wavelength(0.0f),
	amplitude(0.0f),
	speed(0.0f),
	//directionX(0.0f),
	//directionZ(0.0f),
	direction(1.0f, 0.0f, 1.0f),
	frequency(0.0f),
	phase(0.0f)
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

	setParameter(this->timeLoc, this->time);
	setParameter(this->steepnessLoc, this->steepness);
	setParameter(this->wavelengthLoc, this->wavelength);
	setParameter(this->amplitudeLoc, this->amplitude);
	setParameter(this->speedLoc, this->speed);
	//setParameter(this->directionXLoc, this->directionX);
	//setParameter(this->directionZLoc, this->directionZ);
	setParameter(this->directionLoc, this->direction);
	setParameter(this->frequencyLoc, this->frequency);
	setParameter(this->phaseLoc, this->phase);

	Matrix modelViewProj = Cam.getProjectionMatrix() * Cam.getViewMatrix() * modelTransform();
	setParameter(this->modelMatLoc, modelTransform());
	setParameter(this->modelViewProjLoc, modelViewProj);
}

void WaterShader::assignLocations() {
	this->timeLoc = getParameterID("Time");
	this->steepnessLoc = getParameterID("Steepness");
	this->wavelengthLoc = getParameterID("Wavelength");
	this->amplitudeLoc = getParameterID("Amplitude");
	this->speedLoc = getParameterID("Speed");
	//this->directionXLoc = getParameterID("DirectionX");
	//this->directionZLoc = getParameterID("DirectionZ");
	this->directionLoc = getParameterID("Direction");
	this->frequencyLoc = getParameterID("Frequency");
	this->phaseLoc = getParameterID("Phase");

	this->diffuseColorLoc = getParameterID("DiffuseColor");
	this->ambientColorLoc = getParameterID("AmbientColor");
	this->specularColorLoc = getParameterID("SpecularColor");
	this->specularExpLoc = getParameterID("SpecularExp");
	this->diffuseTexLoc = getParameterID("DiffuseTexture");
	this->lightPosLoc = getParameterID("LightPos");
	this->lightColorLoc = getParameterID("LightColor");
	this->eyePosLoc = getParameterID("EyePos");

	this->modelMatLoc = getParameterID("ModelMat");
	this->modelViewProjLoc = getParameterID("ModelViewProjMat");
}