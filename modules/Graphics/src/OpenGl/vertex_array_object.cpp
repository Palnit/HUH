#include "HUH/Graphics/OpenGl/vertex_array_object.h"
namespace HUH::Graphics::OpenGl{

void VertexArrayObject::Bind() {
    if (!m_VAO) {
        glGenVertexArrays(1, &m_VAO);
    }
    glBindVertexArray(m_VAO);
}
void VertexArrayObject::UnBind() {
    glBindVertexArray(0);
}
}