#include "WaterShader.h"
#include <string>

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/Shader/"
#else
#define ASSET_DIRECTORY "../assets/Shader/"
#endif

WaterShader::WaterShader() :
	Time(0.0f),
	Steepness(0.0f),
	Wavelength(0.0f),
	Amplitude(0.0f),
	Speed(0.0f),
	DirectionX(0.0f),
	DirectionZ(0.0f),
	Frequency(0.0f),
	Phase(0.0f)
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

	setParameter(TimeLoc, time());
	setParameter(SteepnessLoc, steepness());
	setParameter(WavelengthLoc, wavelength());
	setParameter(AmplitudeLoc, amplitude());
	setParameter(SpeedLoc, speed());
	setParameter(DirectionXLoc, directionX());
	setParameter(DirectionZLoc, directionZ());

	setParameter(FrequencyLoc, frequency());
	setParameter(PhaseLoc, phase());

	//always update
	Matrix ModelViewProj = Cam.getProjectionMatrix() * Cam.getViewMatrix() * modelTransform();
	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, modelTransform().m);
	glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);
}

void WaterShader::assignLocations() {
	TimeLoc = getParameterID("Time");
	SteepnessLoc = getParameterID("Steepness");
	WavelengthLoc = getParameterID("Wavelength");
	AmplitudeLoc = getParameterID("Amplitude");
	SpeedLoc = getParameterID("Speed");
	DirectionXLoc = getParameterID("DirectionX");
	DirectionZLoc = getParameterID("DirectionZ");
	FrequencyLoc = getParameterID("Frequency");
	PhaseLoc = getParameterID("Phase");

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