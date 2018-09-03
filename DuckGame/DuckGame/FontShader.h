#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <iostream>

#include "color.h"
#include "BaseShader.h"


class FontShader : public BaseShader
{
public:
	FontShader(Vector color);
	virtual void activate(const BaseCamera& camera)	 const;
	Vector getColor() { return this->color; }
    
	void setTexture(GLuint loc, GLuint i);
private:
	Vector color;
	GLuint colorLoc;
	
	GLuint textureLoc;
    
};


