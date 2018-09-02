#include "WaterShader.h"
#include <string>

#define PI 3.14159265358979323846

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

WaterShader::WaterShader(Vector2D waterSize) :
	time(0.0f),
	surfaceTexture(Texture::defaultTex()),
	lightPos(20.0f, 20.0f, 20.0f),
	lightColor(1, 1, 1),
	waterSize(waterSize)
{
	bool loaded = load(ASSET_DIRECTORY"Shader/vswater.glsl", ASSET_DIRECTORY"Shader/fswater.glsl");
	if (!loaded)
		throw std::exception();
	assignLocations();

	this->surfaceTexture->load(ASSET_DIRECTORY"tex/water_surface.png");
}

WaterShader::~WaterShader()
{
}

void WaterShader::activate(const BaseCamera& Cam) const 
{
	BaseShader::activate(Cam);

	setParameter(this->positionLoc, this->position);
	setParameter(this->timeLoc, this->time);
	setParameter(this->waterSizeLoc, this->waterSize);
	setParameter(this->speedLoc, this->speed);

	//waves
	setParameter(this->numWavesLoc, this->numWaves);
	setParameter(this->wavelengthValuesLoc, this->wavelengthValues, this->numWaves);
	setParameter(this->amplitudeValuesLoc, this->amplitudeValues, this->numWaves);
	setParameter(this->speedValuesLoc, this->speedValues, this->numWaves);
	setParameter(this->directionValuesLoc, this->directionValues, this->numWaves);

	//matrices
	Matrix modelViewProj = Cam.getProjectionMatrix() * Cam.getViewMatrix() * modelTransform();
	setParameter(this->modelMatLoc, modelTransform());
	setParameter(this->modelViewProjLoc, modelViewProj);

	setParameter(eyePosLoc, Cam.position());

	//visual variables
	setParameter(this->lightPosLoc, this->lightPos);
	setParameter(this->lightColorLoc, this->lightColor);

	this->surfaceTexture->activate(0);
	setParameter(this->surfaceTextureLoc, 0);
}

void WaterShader::assignLocations() 
{
	this->positionLoc = getParameterID("Position");
	this->timeLoc = getParameterID("Time");
	this->waterSizeLoc = getParameterID("WaterSize");
	this->speedLoc = getParameterID("Speed");

	this->numWavesLoc = getParameterID("numWaves");
	this->wavelengthValuesLoc = getParameterID("WavelengthValues");
	this->amplitudeValuesLoc = getParameterID("AmplitudeValues");
	this->speedValuesLoc = getParameterID("SpeedValues");
	this->directionValuesLoc = getParameterID("DirectionValues");

	this->lightPosLoc = getParameterID("LightPos");
	this->lightColorLoc = getParameterID("LightColor");
	this->eyePosLoc = getParameterID("EyePos");

	this->surfaceTextureLoc = getParameterID("SurfaceTexture");

	this->modelMatLoc = getParameterID("ModelMat");
	this->modelViewProjLoc = getParameterID("ModelViewProjMat");
}

void WaterShader::setWaves(Wave waves[MAX_WAVES], int num) 
{
	if (num > MAX_WAVES) num = MAX_WAVES;
	for (int i = 0; i < num; ++i) {
		this->wavelengthValues[i] = waves[i].wavelength;
		this->amplitudeValues[i] = waves[i].amplitude;
		this->speedValues[i] = waves[i].speed;
		this->directionValues[i] = waves[i].direction;
	}
	this->numWaves = num;
}