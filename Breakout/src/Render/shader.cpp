#include "shader.h"


#include <iostream>

Shader& Shader::use()
{
    glUseProgram(this->ID);
    return *this;
}

void Shader::compile(const char* vertex_source, const char* fragment_source, const char* geometry_source)
{
    unsigned int s_vertex, s_fragment, g_shader;

    // Vertex shader
    s_vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(s_vertex, 1, &vertex_source, nullptr);
    glCompileShader(s_vertex);
    check_compile_errors(s_vertex, "VERTEX");

    // Fragment shader
    s_fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(s_fragment, 1, &fragment_source, nullptr);
    glCompileShader(s_fragment);
    check_compile_errors(s_fragment, "FRAGMENT");

	// 	If the source code of the geometry shader is given, we also compile it
    if (geometry_source != nullptr)
    {
        g_shader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(g_shader, 1, &geometry_source, nullptr);
        glCompileShader(g_shader);
        check_compile_errors(g_shader, "GEOMETRY");
    }

    // Shader program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, s_vertex);
    glAttachShader(this->ID, s_fragment);
    if (geometry_source != nullptr)
        glAttachShader(this->ID, g_shader);
    glLinkProgram(this->ID);
    check_compile_errors(this->ID, "PROGRAM");

    glDeleteShader(s_vertex);
    glDeleteShader(s_fragment);
    if (geometry_source != nullptr)
        glDeleteShader(g_shader);
}

void Shader::set_float(const char* name, float value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform1f(glGetUniformLocation(this->ID, name), value);
}
void Shader::set_integer(const char* name, int value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform1i(glGetUniformLocation(this->ID, name), value);
}
void Shader::set_vector_2f(const char* name, float x, float y, bool useShader)
{
    if (useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}
void Shader::set_vector_2f(const char* name, const glm::vec2& value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}
void Shader::set_vector_3f(const char* name, float x, float y, float z, bool useShader)
{
    if (useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}
void Shader::set_vector_3f(const char* name, const glm::vec3& value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}
void Shader::set_vector_4f(const char* name, float x, float y, float z, float w, bool useShader)
{
    if (useShader)
        this->use();
    glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}
void Shader::set_vector_4f(const char* name, const glm::vec4& value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}
void Shader::set_matrix4(const char* name, const glm::mat4& matrix, bool useShader)
{
    if (useShader)
        this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(matrix));
}


void Shader::check_compile_errors(unsigned int object, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, nullptr, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, nullptr, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}