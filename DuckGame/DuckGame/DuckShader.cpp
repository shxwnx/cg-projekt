#include "DuckShader.h"

#define SHADER_DIRECTORY "../../assets/Shader/"

DuckShader::DuckShader()
{
	//this->color = color;
	bool loaded = load(SHADER_DIRECTORY "vsduck.glsl", SHADER_DIRECTORY "fsduck.glsl");
	if (!loaded)
		throw std::exception();

	//this->colorId = getParameterID("Color");
}

void DuckShader::activate(const BaseCamera& Cam)
{
	//setParameter(this->colorId, this->color);
	BaseShader::activate(Cam);
}
