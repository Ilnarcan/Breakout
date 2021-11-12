#ifndef _SHADER_H
#define _SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>


class Shader
{
public:
	unsigned int ID;

	Shader()
	{
	}

	Shader& use();

	void compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

	void set_float(const char* name, float value, bool useShader = false);
	void set_integer(const char* name, int value, bool useShader = false);
	void set_vector_2f(const char* name, float x, float y, bool useShader = false);
	void set_vector_2f(const char* name, const glm::vec2& value, bool useShader = false);
	void set_vector_3f(const char* name, float x, float y, float z, bool useShader = false);
	void set_vector_3f(const char* name, const glm::vec3& value, bool useShader = false);
	void set_vector_4f(const char* name, float x, float y, float z, float w, bool useShader = false);
	void set_vector_4f(const char* name, const glm::vec4& value, bool useShader = false);
	void set_matrix4(const char* name, const glm::mat4& matrix, bool useShader = false);

private:
	void check_compile_errors(unsigned int object, std::string type);
};

#endif
