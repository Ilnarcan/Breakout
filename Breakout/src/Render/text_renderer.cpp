/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include <iostream>

#include <include/ft2build.h>
#include FT_FREETYPE_H

#include "text_renderer.h"


TextRenderer::TextRenderer(const Shader& shader)
{
    this->m_text_shader = shader;

    // Loading VAO / VBO for texture rectangles
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextRenderer::load(std::string font, unsigned int font_size)
{
    this->m_characters.clear();

    // Initialize and load the FreeType library
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) 
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    // Loading the font as face
    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    // Set the size of loaded glyphs
    FT_Set_Pixel_Sizes(face, 0, font_size);

    // Disable the byte alignment constraint
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 255; c++)
    {
        // Loading a glyph symbol
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // Texture generation
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
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

        // Setting texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Save symbols for future use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        m_characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);


    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::render_text(std::string text, float x, float y, float scale, bool align_ñenter, glm::vec3 color)
{
    // Activating the appropriate render state
    this->m_text_shader.use();
    this->m_text_shader.set_vector_3f("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);

    if (align_ñenter) {

        float size_of_text_x = 0.0f;

        for(auto i : text) {
            const Character ch = m_characters[i];
            size_of_text_x += (ch.m_advance >> 6) * scale;
        }

        x = (x - size_of_text_x) / 2.0f;

    }

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); ++c)
    {
        Character ch = m_characters[*c];

        const float xpos = x + ch.n_bearing.x * scale;
        const float ypos = y + (this->m_characters['H'].n_bearing.y - ch.n_bearing.y) * scale;

        const float w = ch.m_size.x * scale;
        const float h = ch.m_size.y * scale;

        // Updating VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f },
            { xpos,     ypos,       0.0f, 0.0f },

            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f }
        };

        // Render a glyph texture to a rectangle
        glBindTexture(GL_TEXTURE_2D, ch.m_texture_id);

        // Updating the contents of the VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render a rectangle
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Move the cursor to the next glyph
        x += (ch.m_advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

