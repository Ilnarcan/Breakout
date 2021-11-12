#ifndef _ASSET_MANAGER_H
#define _ASSET_MANAGER_H

#include <map>
#include <string>
#include "ECS.h"
#include "texture.h"
#include "shader.h"


class AssetManager
{
public:
	AssetManager(Manager* man);
	~AssetManager();


	//texture management
	void add_texture(std::string id, const char* path, bool alpha = true);
	Texture2D* get_texture(std::string id);
	static Texture2D* load_texture_from_file(const char* file, bool alpha);

	//shader management
	static Shader load_shader_from_file(const char* v_shader_file, const char* f_shader_file, const char* g_shader_file);
	Shader load_shader(const char* v_shader_file, const char* f_shader_file, const char* g_shader_file,
	                   std::string name);
	Shader get_shader(std::string name);

	void clear();

private:
	Manager* manager;
	std::map<std::string, Texture2D*> m_textures;
	std::map<std::string, Shader> m_shaders;
};

#endif
