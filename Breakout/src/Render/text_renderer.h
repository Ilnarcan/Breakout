#ifndef _TEXT_RENDERER_H
#define _TEXT_RENDERER_H

#include <map>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"


struct Character
{
	unsigned int m_texture_id;
	glm::ivec2 m_size;
	glm::ivec2 n_bearing;
	unsigned int m_advance;
};

class TextRenderer
{
public:
	TextRenderer(const Shader& shader);

	void load(std::string font, unsigned int font_size);
	void render_text(std::string text, float x, float y, float scale, bool align_center = false,
	                 glm::vec3 color = glm::vec3(1.0f));

	std::map<char, Character> m_characters;
	Shader m_text_shader;

private:
	unsigned int VAO, VBO;
};

#endif
