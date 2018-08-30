#pragma once

#define MAX_WAVES 5

#ifndef WaterShader_hpp
#define WaterShader_hpp

#include <stdio.h>
#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include <iostream>
#include <assert.h>
#include "color.h"
#include "vector.h"
#include "matrix.h"
#include "camera.h"
#include "baseshader.h"
#include "texture.h"
#include "Wave.h"

class WaterShader : public BaseShader
{
public:
	WaterShader();
	~WaterShader();

	float getTime() const { return this->time; }
	void setTime(const float t) { this->time = t; }
	void setWaves(Wave waves[MAX_WAVES], int num);

	virtual void activate(const BaseCamera& Cam) const;

protected:
	void assignLocations();
	void renderReflection();
	void renderRefractionAndDepth();

private:
	//variables...
	Texture* reflectionTexture;
	Texture* refractionTexture;
	Texture* depthTexture;

	Vector position;
	float time;

	//float steepnessValues[MAX_WAVES];
	int numWaves;
	float wavelengthValues[MAX_WAVES];	//crest-to-crest distance between waves in world space
	float amplitudeValues[MAX_WAVES];	//height from the water plane to the wave crest
	float speedValues[MAX_WAVES];		//distance the crest moves forward per second
	Vector directionValues[MAX_WAVES];	//horizontal vector perpendicular to the wave front along which the crest travels

	Color diffuseColor;
	Color specularColor;
	Color ambientColor;
	float specularExp;
	Vector lightPos;
	Color lightColor;


	GLint positionLoc;
	GLint timeLoc;
	//GLint steepnessLoc;	

	GLint numWavesLoc;
	//GLint steepnessValuesLoc;
	GLint wavelengthValuesLoc;
	GLint amplitudeValuesLoc;
	GLint speedValuesLoc;
	GLint directionValuesLoc;

	GLint diffuseColorLoc;
	GLint specularColorLoc;
	GLint ambientColorLoc;
	GLint specularExpLoc;
	GLint lightPosLoc;
	GLint lightColorLoc;

	GLint modelMatLoc;
	GLint modelViewProjLoc;
	GLint eyePosLoc;
};

#endif /* WaterShader_hpp */