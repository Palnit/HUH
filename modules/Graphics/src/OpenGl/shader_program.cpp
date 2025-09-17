#include "HUH/Graphics/OpenGl/shader_program.h"

namespace HUH::Graphics::OpenGl{
ShaderProgram::ShaderProgram() : m_program(0) {}

void ShaderProgram::AttachShader(GLuint shader) {
    CreateProgram();
    m_shaders.push_back(shader);
    glAttachShader(m_program, shader);
}
void ShaderProgram::Bind() {
    CreateProgram();
    LinkProgram();
    glUseProgram(m_program);
}
void ShaderProgram::UnBind() { glUseProgram(0); }

void ShaderProgram::LinkProgram() {
    if (linked) { return; }
    CreateProgram();
    linked = true;
    glLinkProgram(m_program);
}

void ShaderProgram::CreateProgram() {
    if (m_program) { return; }
    m_program = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
    for (const auto shader : m_shaders) {
        glDetachShader(m_program, shader);
        glDeleteShader(shader);
    }
    if (m_program) { glDeleteProgram(m_program); }
}
}// namespace HUH