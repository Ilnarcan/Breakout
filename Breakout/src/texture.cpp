#include <iostream>

#include "texture.h"


Texture2D::Texture2D()
	: m_width(0), m_height(0), m_data(nullptr), m_internal_format(GL_RGB), m_image_format(GL_RGB), m_wrap_S(GL_REPEAT),
	  m_wrap_T(GL_REPEAT), m_filter_min(GL_LINEAR), m_filter_max(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
}

void Texture2D::generate(unsigned int width, unsigned int height, unsigned char* data)
{
	this->m_width = width;
	this->m_height = height;
	this->m_data = data;

	// Create a texture
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->m_internal_format, width, height, 0, this->m_image_format, GL_UNSIGNED_BYTE,
	             data);

	// Set the blending and filtering modes for the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->m_wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->m_wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->m_filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->m_filter_max);

	glGenerateMipmap(GL_TEXTURE_2D);

	// Detaching the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}
