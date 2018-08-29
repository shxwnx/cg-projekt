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

	virtual void activate(const BaseCamera& Cam) const;

protected:
	void assignLocations();

private:
	//variables...
	float time;
	float steepness;	//pinch on the wave 
	float wavelength;	//crest-to-crest distance between waves in world space
	float amplitude;	//height from the water plane to the wave crest
	float speed;		//distance the crest moves forward per second
	//float directionX;	//horizontal vector perpendicular to the wave front along which the crest travels
	//float directionZ;
	Vector direction;
	float frequency;
	float phase;

	GLint timeLoc;
	GLint steepnessLoc;	
	GLint wavelengthLoc;	
	GLint amplitudeLoc;	
	GLint speedLoc;		
	//GLint directionXLoc;	
	//GLint directionZLoc;
	GLint directionLoc;
	GLint frequencyLoc;
	GLint phaseLoc;

	Color diffuseColor;
	Color specularColor;
	Color ambientColor;
	float specularExp;
	Vector lightPos;
	Color lightColor;
	const Texture* diffuseTexture;

	GLint diffuseColorLoc;
	GLint specularColorLoc;
	GLint ambientColorLoc;
	GLint specularExpLoc;
	GLint lightPosLoc;
	GLint lightColorLoc;
	GLint modelMatLoc;
	GLint modelViewProjLoc;
	GLint eyePosLoc;
	GLint diffuseTexLoc;
};

#endif /* WaterShader_hpp */