#include "UIService.h"


UIService::UIService(const char* font)
{
	//this->application = application;
	//this->spawner = spawner;

	FT_Library ftLibrary;
	if (FT_Init_FreeType(&ftLibrary))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face ftFace;
	if (FT_New_Face(ftLibrary, font, 0, &ftFace))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(ftFace, 0, 48);

	if (FT_Load_Char(ftFace, 'X', FT_LOAD_RENDER))
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(ftFace, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			ftFace->glyph->bitmap.width,
			ftFace->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			ftFace->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			Vector2D(ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows),
			Vector2D(ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top),
			ftFace->glyph->advance.x
		};
		this->characters.insert(std::pair<GLchar, Character>(c, character));
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	FT_Done_Face(ftFace);
	FT_Done_FreeType(ftLibrary);
}

UIService::~UIService()
{
	this->ui.clear();
}

bool UIService::loadUI(std::vector<const char*> files)
{
	if (files.size() <= 0 || this->ui.size() <= 0)
		return false;

	for (auto file : files) {
		auto model = new Model(file, false);
		if (!model->load(file, false)) {
			return false;
		}
		model->shader(this->pShader, false);
		this->ui.push_back(model);
	}
	return true;
}

void UIService::update(float dtime)
{			  
	//this->timePassed = this->application->getTimePassed();
	//this->spawnTime = this->spawner->getSpawnTime();
	//this->speed = this->spawner->getSpeed();
	//this->objectsDodged = this->spawner->getObjectsDodged();

	//renderText("harro", 50.0f, 50.0f, 5.0f, Vector(0.0f, 0.0f, 0.0f));
}

void UIService::draw(const BaseCamera & camera)
{
	for (auto model : this->ui) {
		model->draw(camera);
	}
}

void UIService::renderText( std::string text, GLfloat x, GLfloat y, GLfloat scale, Vector color)
{
	// Activate corresponding render state	
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = this->characters[*c];

		GLfloat xpos = x + ch.Bearing.X * scale;
		GLfloat ypos = y - (ch.Size.Y - ch.Bearing.Y) * scale;

		GLfloat w = ch.Size.X * scale;
		GLfloat h = ch.Size.Y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos,     ypos,       0.0, 1.0 },
		{ xpos + w, ypos,       1.0, 1.0 },

		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)


	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

