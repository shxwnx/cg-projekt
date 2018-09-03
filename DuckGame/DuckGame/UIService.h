//aus Text-Rendering Tutorial (siehe readme.pdf)

#pragma once

#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H  

#include "BaseModel.h"
#include "Model.h"
#include "Vector2D.h"



struct Character {
	GLuint     TextureID;  // ID handle of the glyph texture
	Vector2D Size;       // Size of glyph
	Vector2D Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph

};

class UIService : public BaseModel {


public:

	UIService(const char* font, Camera* camera);
	virtual ~UIService();
	bool loadUI(std::vector<const char*> files);
	void update(float dtime);
	virtual void draw(const BaseCamera& camera);

private:
	std::vector<Model*> ui;
	Camera* camera;

	std::map<GLchar, Character> characters;
	GLuint textureLoc;
	GLuint VAO;
	GLuint VBO;

	// UI
	float timePassed;
	float spawnTime;
	float speed;
	int objectsDodged;


	void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale);

};