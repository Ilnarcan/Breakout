#include <iostream>
#include <sstream>
#include <fstream>

#include "asset_manager.h"
#include "stb_image.h"


AssetManager::AssetManager(Manager* man) : manager(man)
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::add_texture(std::string id, const char* path, bool alpha)
{
	m_textures.emplace(id, load_texture_from_file(path, alpha));
}

Shader AssetManager::get_shader(std::string name)
{
	return m_shaders[name];
}

Shader AssetManager::load_shader(const char* v_shader_file, const char* f_shader_file, const char* g_shader_file,
                                 std::string name)
{
	m_shaders[name] = load_shader_from_file(v_shader_file, f_shader_file, g_shader_file);
	return m_shaders[name];
}

Shader AssetManager::load_shader_from_file(const char* v_shader_file, const char* f_shader_file,
                                           const char* g_shader_file)
{
	// Get vertex / fragment shader source code from filePath variable
	std::string vertex_code;
	std::string fragment_code;
	std::string geometry_code;
	try
	{
		// Opening files
		std::ifstream vertex_shader_file(v_shader_file);
		std::ifstream fragment_shader_file(f_shader_file);
		std::stringstream v_shader_stream, f_shader_stream;

		// We read the contents of the file buffer into the corresponding streams
		v_shader_stream << vertex_shader_file.rdbuf();
		f_shader_stream << fragment_shader_file.rdbuf();

		vertex_shader_file.close();
		fragment_shader_file.close();

		// Convert stream to string
		vertex_code = v_shader_stream.str();
		fragment_code = f_shader_stream.str();

		// If a geometry shader path is present, then load it as well
		if (g_shader_file != nullptr)
		{
			std::ifstream geometry_shader_file(g_shader_file);
			std::stringstream g_shader_stream;
			g_shader_stream << geometry_shader_file.rdbuf();
			geometry_shader_file.close();
			geometry_code = g_shader_stream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const char* v_shader_code = vertex_code.c_str();
	const char* f_shader_code = fragment_code.c_str();
	const char* g_shader_code = geometry_code.c_str();

	// Create a shader object from source
	Shader shader;
	shader.compile(v_shader_code, f_shader_code, g_shader_file != nullptr ? g_shader_code : nullptr);
	return shader;
}

Texture2D* AssetManager::load_texture_from_file(const char* file, bool alpha)
{
	// Create a texture object
	const auto texture = new Texture2D();
	if (alpha)
	{
		texture->m_internal_format = GL_RGBA;
		texture->m_image_format = GL_RGBA;
	}

	// Loading the image
	int width, height, channels;
	unsigned char* data = stbi_load(file, &width, &height, &channels, 0);

	texture->generate(width, height, data);

	stbi_image_free(data);
	return texture;
}

Texture2D* AssetManager::get_texture(std::string id)
{
	return m_textures[id];
}

void AssetManager::clear()
{
	for (const auto iter : m_shaders)
		glDeleteProgram(iter.second.ID);

	for (const auto iter : m_textures)
		glDeleteTextures(1, &iter.second->ID);
}
