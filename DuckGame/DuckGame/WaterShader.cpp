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
	reflectionTexture(Texture::defaultTex()),
	refractionTexture(Texture::defaultTex()),
	surfaceTexture(Texture::defaultTex())
{
	bool loaded = load(ASSET_DIRECTORY"Shader/vswater.glsl", ASSET_DIRECTORY"Shader/fswater.glsl");
	if (!loaded)
		throw std::exception();
	assignLocations();

	this->surfaceTexture->load(ASSET_DIRECTORY"tex/water_surface.png");

	this->waterSize = waterSize;
}

WaterShader::~WaterShader()
{

}

void WaterShader::activate(const BaseCamera& Cam) const {
	BaseShader::activate(Cam);

	setParameter(this->depthTextureLoc, *this->depthTexture);

	setParameter(this->positionLoc, this->position);
	setParameter(this->timeLoc, this->time);
	setParameter(this->waterSizeLoc, this->waterSize);

	//Waves
	setParameter(this->numWavesLoc, this->numWaves);
	setParameter(this->wavelengthValuesLoc, this->wavelengthValues, this->numWaves);
	setParameter(this->amplitudeValuesLoc, this->amplitudeValues, this->numWaves);
	setParameter(this->speedValuesLoc, this->speedValues, this->numWaves);
	setParameter(this->directionValuesLoc, this->directionValues, this->numWaves);

	//Projection Matrix
	Matrix modelViewProj = Cam.getProjectionMatrix() * Cam.getViewMatrix() * modelTransform();
	setParameter(this->modelMatLoc, modelTransform());
	setParameter(this->modelViewProjLoc, modelViewProj);

	setParameter(eyePosLoc, Cam.position());

	//Visual Variables
	setParameter(this->diffuseColorLoc, this->diffuseColor);
	setParameter(this->ambientColorLoc, this->ambientColor);
	setParameter(this->specularColorLoc, this->specularColor);
	setParameter(this->specularExpLoc, this->specularExp);
	setParameter(this->lightPosLoc, this->lightPos);
	setParameter(this->lightColorLoc, this->lightColor);

	this->surfaceTexture->activate(0);
	setParameter(this->surfaceTextureLoc, 0);
}

void WaterShader::assignLocations() {
	this->depthTextureLoc = getParameterID("DepthTexture");
	
	this->positionLoc = getParameterID("Position");
	this->timeLoc = getParameterID("Time");
	this->waterSizeLoc = getParameterID("WaterSize");

	this->numWavesLoc = getParameterID("numWaves");
	this->wavelengthValuesLoc = getParameterID("WavelengthValues");
	this->amplitudeValuesLoc = getParameterID("AmplitudeValues");
	this->speedValuesLoc = getParameterID("SpeedValues");
	this->directionValuesLoc = getParameterID("DirectionValues");

	this->diffuseColorLoc = getParameterID("DiffuseColor");
	this->ambientColorLoc = getParameterID("AmbientColor");
	this->specularColorLoc = getParameterID("SpecularColor");
	this->specularExpLoc = getParameterID("SpecularExp");
	this->lightPosLoc = getParameterID("LightPos");
	this->lightColorLoc = getParameterID("LightColor");
	this->eyePosLoc = getParameterID("EyePos");

	this->surfaceTextureLoc = getParameterID("SurfaceTexture");

	this->modelMatLoc = getParameterID("ModelMat");
	this->modelViewProjLoc = getParameterID("ModelViewProjMat");
}

void WaterShader::setWaves(Wave waves[MAX_WAVES], int num) {
	if (num > MAX_WAVES) num = MAX_WAVES;
	for (int i = 0; i < num; ++i) {
		this->wavelengthValues[i] = waves[i].wavelength;
		this->amplitudeValues[i] = waves[i].amplitude;
		this->speedValues[i] = waves[i].speed;
		this->directionValues[i] = waves[i].direction;
	}
	this->numWaves = num;

}

void WaterShader::renderReflection(const Camera& Cam) {
	glViewport(0, 0, 2048, 2048); //////HIER//texSize definieren
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//gluLookAt(Cam.position().X, Cam.position().Y, Cam.position().Z, 
		//Cam.target().X, Cam.target().Y, Cam.target().Z,
		//Cam.up().X, Cam.up().Y, Cam.up().Z);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glScalef(1.0, -1.0, 1.0);
	double plane[4] = { 0.0, 1.0, 0.0, 0.0 }; //water at y=0
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, plane);
	//RenderScene();//////HIER
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	//render reflection to texture
	this->reflectionTexture->bindTexture(GL_TEXTURE_2D);
	//glCopyTexSubImage2D copies the frame buffer
	//to the bound texture
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 2048, 2048);//////HIER//texSize definieren
}

void WaterShader::renderRefractionAndDepth(){
	//glViewport(0, 0, texSize, texSize);//////HIER//texSize definieren
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//gluLookAt(); //////HIER

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	//normal pointing along negative y
	double plane[4] = { 0.0, -1.0, 0.0, 0.0 };
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, plane);
	//RenderScene();//////HIER
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	//render color buffer to texture
	this->refractionTexture->bindTexture(GL_TEXTURE_2D);
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, texSize, texSize);//////HIER//texSize definieren

	//render depth to texture
	//this->depthTexture->bindTexture(GL_TEXTURE_2D);
	//glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, texSize, texSize, 0);//////HIER//texSize definieren
}