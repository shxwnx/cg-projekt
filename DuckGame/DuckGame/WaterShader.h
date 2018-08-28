#pragma once

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

class WaterShader : public BaseShader
{
public:
	WaterShader();
	~WaterShader();

	//getter and setter...
	float time() const { return Time; }
	float steepness() const { return Steepness; }
	float wavelength() const { return Wavelength; }
	float amplitude() const { return Amplitude; }
	float speed() const { return Speed; }
	float directionX() const { return DirectionX; }
	float directionZ() const { return DirectionZ; }
	float frequency() const { return Frequency; }
	float phase() const { return Phase; }

	void time(float time) { this->Time = time; }
	void steepness(float steepness) { this->Steepness = steepness; }
	void wavelength(float wavelength) { this->Wavelength = wavelength; }
	void amplitude(float amplitude) { this->Amplitude = amplitude; }
	void speed(float speed) { this->Speed = speed; }
	void directionX(float directionX) { this->DirectionX = directionX; }
	void directionZ(float directionZ) { this->DirectionZ = directionZ; }
	void frequency(float frequency) { this->Frequency = frequency; }
	void phase(float phase) { this->Phase = phase; }

	virtual void activate(const BaseCamera& Cam) const;

protected:
	void assignLocations();

private:
	//variables...
	float Time;
	float Steepness;	//pinch on the wave 
	float Wavelength;	//crest-to-crest distance between waves in world space
	float Amplitude;	//height from the water plane to the wave crest
	float Speed;		//distance the crest moves forward per second
	float DirectionX;	//horizontal vector perpendicular to the wave front along which the crest travels
	float DirectionZ;
	float Frequency;
	float Phase;

	GLint TimeLoc;
	GLint SteepnessLoc;	//pinch on the wave 
	GLint WavelengthLoc;	//crest-to-crest distance between waves in world space
	GLint AmplitudeLoc;	//height from the water plane to the wave crest
	GLint SpeedLoc;		//distance the crest moves forward per second
	GLint DirectionXLoc;	//horizontal vector perpendicular to the wave front along which the crest travels
	GLint DirectionZLoc;
	GLint FrequencyLoc;
	GLint PhaseLoc;

	GLint DiffuseColorLoc;
	GLint SpecularColorLoc;
	GLint AmbientColorLoc;
	GLint SpecularExpLoc;
	GLint LightPosLoc;
	GLint LightColorLoc;
	GLint ModelMatLoc;
	GLint ModelViewProjLoc;
	GLint EyePosLoc;
	GLint DiffuseTexLoc;
};

#endif /* WaterShader_hpp */