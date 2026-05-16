#include "HUH/Graphics/OpenGl/texture.h"
namespace HUH::Graphics::OpenGl{

Texture2D::Texture2D(SDL_Surface* surface)
    : m_width(surface->w),
      m_height(surface->h),
      m_data(surface->pixels) {
    glGenTextures(1, &m_tex);
    glBindTexture(m_target, m_tex);
    glTexImage2D(m_target, m_level, m_internalformat, m_width, m_height, 0,
                 m_format, m_type, m_data);
    glBindTexture(m_target, 0);
}
void Texture2D::Bind(const size_t TextureUnit) const {
    glActiveTexture(GL_TEXTURE0 + TextureUnit);
    glBindTexture(m_target, m_tex);
}
void Texture2D::UnBind() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(m_target, 0);
}

void Texture2D::SetParameters(
    const std::vector<ITextureParameters*>& parameters) const {
    glBindTexture(m_target, m_tex);
    for (const auto& Param : parameters) { Param->ApplyParameter(); }
    glBindTexture(m_target, 0);
}
Texture2D::~Texture2D() { glDeleteTextures(1, &m_tex); }
}// namespace HUH