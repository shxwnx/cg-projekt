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

class WaterShader : BaseShader
{
public:
	WaterShader();
	~WaterShader();

	//getter and setter...

	virtual void activate(const BaseCamera& Cam) const;

protected:
	void assignLocations();

private:
	//variables...

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