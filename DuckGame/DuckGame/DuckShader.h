#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <iostream>

#include "BaseShader.h"
#include "Texture.h"

class DuckShader : public BaseShader
{
public:
	DuckShader();

	const Color& getDiffuseColor() const { return this->diffuseColor; }
	const Color& getAmbientColor() const { return this->ambientColor; }
	const Color& getSpecularColor() const { return this->specularColor; }
	float getSpecularExp() const { return this->specularExp; }
	const Texture* getDiffuseTexture() const { return this->diffuseTexture; }
	const Vector& getLightPos() const { return this->lightPos; }
	const Color& getLightColor() const { return this->lightColor; }

	void setDiffuseColor(const Color& c) { this->diffuseColor = c; }
	void setAmbientColor(const Color& c) { this->ambientColor = c; }
	void setSpecularColor(const Color& c) { this->specularColor = c; }
	void setSpecularExp(float exp) { this->specularExp = exp; }
	void setDiffuseTexture(const Texture* pTex) { this->diffuseTexture = pTex; }
	void setLightPos(const Vector& pos) { this->lightPos = pos; }
	void setLightColor(const Color& c) { this->lightColor = c; }


	virtual void activate(const BaseCamera& Cam) const;
    
    
private:
	Color diffuseColor;
	Color specularColor;
	Color ambientColor;
	float specularExp;
	Vector lightPos;
	Color lightColor;
	const Texture* diffuseTexture;

	GLuint modelMatLoc;
	GLuint modelViewProjLoc;

	GLint diffuseColorLoc;
	GLint specularColorLoc;
	GLint ambientColorLoc;
	GLint specularExpLoc;
	GLint lightPosLoc;
	GLint lightColorLoc;
	GLint eyePosLoc;
	GLint diffuseTexLoc;


    
};


