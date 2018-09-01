#include "UIService.h"
#include "FontShader.h"


UIService::UIService(const char* font, Camera* camera)
{
	this->camera = camera;
	// Activate corresponding render state	
	auto shader = new FontShader(Vector(1.0f, 0.0f, 0.0f));
	this->shader(shader, false);
	//FontShader* shader = dynamic_cast<FontShader*>(this->shader());


	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, font, 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
	
		glGenTextures(1, &textureLoc);
		glBindTexture(GL_TEXTURE_2D, textureLoc);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			textureLoc,
			Vector2D(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			Vector2D(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		this->characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
		model->shader(this->shader(), false);
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


}

void UIService::draw(const BaseCamera & camera)
{
	//this->shader()->activate(*this->camera);
	this->renderText("HARRO", 0 , 0, 0.1f);
	for (auto model : this->ui) {
		model->draw(camera);
	}
}

void UIService::renderText( std::string text, GLfloat x, GLfloat y, GLfloat scale)
{
			
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

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
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	//shader->setBlock()
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

