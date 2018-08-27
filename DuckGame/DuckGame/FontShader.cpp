#include "FontShader.h"

#define SHADER_DIRECTORY "../../assets/Shader/"

FontShader::FontShader(Vector color)
{
	this->color = color;
	bool loaded = load(SHADER_DIRECTORY "vsfont.glsl", SHADER_DIRECTORY "fsfont.glsl");
	if (!loaded)
		throw std::exception();

	this->colorId = getParameterID("Color");
}

void FontShader::activate()
{
	setParameter(this->colorId, this->color);
	Camera cam;
	BaseShader::activate(cam);
}
