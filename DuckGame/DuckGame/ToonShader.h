#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <iostream>

#include "BaseShader.h"
#include "Texture.h"

#define OBJECT 0
#define POOL 1

class ToonShader : public BaseShader
{
public:
	ToonShader();

	const Color& getDiffuseColor() const { return this->diffuseColor; }
	const Color& getAmbientColor() const { return this->ambientColor; }
	const Color& getSpecularColor() const { return this->specularColor; }
	float getSpecularExp() const { return this->specularExp; }
	const Texture* getDiffuseTexture() const { return this->diffuseTexture; }
	const Vector& getLightPos() const { return this->lightPos; }
	const Color& getLightColor() const { return this->lightColor; }
	float getSpeed() const { return this->speed; }

	void setDiffuseColor(const Color& c) { this->diffuseColor = c; }
	void setAmbientColor(const Color& c) { this->ambientColor = c; }
	void setSpecularColor(const Color& c) { this->specularColor = c; }
	void setSpecularExp(float exp) { this->specularExp = exp; }
	void setDiffuseTexture(const Texture* pTex) { this->diffuseTexture = pTex; }
	void setLightPos(const Vector& pos) { this->lightPos = pos; }
	void setLightColor(const Color& c) { this->lightColor = c; }
	void setType(int t) { this->type = t; }
	void setSpeed(const float s) { this->speed = s; }

	void addTime(float time) { this->time += time; }

	virtual void activate(const BaseCamera& Cam) const;
    
    
private:
	int type;
	float speed;

	Color diffuseColor;
	Color specularColor;
	Color ambientColor;
	float specularExp;
	Vector lightPos;
	Color lightColor;
	const Texture* diffuseTexture;
	float time;

	GLuint modelMatLoc;
	GLuint modelViewProjLoc;

	GLint typeLoc;
	GLint diffuseColorLoc;
	GLint specularColorLoc;
	GLint ambientColorLoc;
	GLint specularExpLoc;
	GLint lightPosLoc;
	GLint lightColorLoc;
	GLint eyePosLoc;
	GLint diffuseTexLoc;

	GLint speedLoc;
	GLuint timeLoc;


    
};


