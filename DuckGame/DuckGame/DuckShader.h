#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <iostream>

#include "BaseShader.h"


class DuckShader : public BaseShader
{
public:
	DuckShader();
    virtual void activate(const BaseCamera& Cam) ;
    
    
private:

    
};


