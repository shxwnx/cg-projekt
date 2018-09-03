#include "FontShader.h"
#define SHADER_DIRECTORY "../../assets/Shader/"

FontShader::FontShader(Vector color)
{
	this->color = color;

	bool loaded = load(SHADER_DIRECTORY "vsfont.glsl", SHADER_DIRECTORY "fsfont.glsl");
	if (!loaded)
		throw std::exception();

	this->colorLoc = getParameterID("Color");
	this->textureLoc = getParameterID("Texture");
}

void FontShader::setTexture(GLuint loc, GLuint i) {
	this->setParameter(loc, i);
}

void FontShader::activate(const BaseCamera& camera)	 const
{
	BaseShader::activate(camera);

	Matrix proj = camera.getProjectionMatrix();

	setParameter(this->colorLoc, this->color);
	//setParameter(this->projectionLoc, proj.orthographic(600.0f,900.0f,1,1));
}
