#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <GL/glew.h>

class Texture2D
{
public:
	Texture2D();

	void generate(unsigned int width, unsigned int height, unsigned char* data);

	void bind() const;

	unsigned int ID;

	unsigned int m_width, m_height;
	unsigned char* m_data;

	unsigned int m_internal_format;
	unsigned int m_image_format;

	unsigned int m_wrap_S; // S - axis blend mode
	unsigned int m_wrap_T; // T - axis blend mode
	unsigned int m_filter_min; // filtering mode if texture pixels < screen pixels
	unsigned int m_filter_max; //  filtering mode if texture pixels > screen pixels
};

#endif
