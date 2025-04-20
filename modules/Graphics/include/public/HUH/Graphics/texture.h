#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <SDL3/SDL_surface.h>

#include <functional>
#include <memory>
#include <tuple>
#include <vector>

namespace HUH {

class ITextureParameters {
public:
    virtual ~ITextureParameters() = default;
    virtual void ApplyParameter() = 0;
};

template<typename Function, typename... Args>
class TextureParameters : public ITextureParameters {
public:
    explicit TextureParameters(Function&& function, Args&&... args)
        : m_function(std::forward<Function>(function)),
          m_args(std::forward<Args>(args)...) {}

    void ApplyParameter() override { std::apply(m_function, m_args); }

private:
    std::function<Function> m_function;
    std::tuple<Args...> m_args;
};

class Texture2D {
public:
    explicit Texture2D(SDL_Surface* surface);
    void Bind(size_t TextureUnit) const;
    void UnBind() const;
    template<typename Function, typename... Args>
    void SetParameter(Function&& function, Args&&... args) {
        glBindTexture(m_target, m_tex);
        function(m_target, args...);
        glBindTexture(m_target, 0);
    }
    void SetParameters(
        const std::vector<ITextureParameters*>& parameters) const;
    ~Texture2D();

private:
    GLuint m_tex = 0;
    GLenum m_target = GL_TEXTURE_2D;
    GLint m_level = 0;
    GLint m_internalformat = GL_RGBA;
    GLsizei m_width = 0;
    GLsizei m_height = 0;
    GLenum m_format = GL_RGBA;
    GLenum m_type = GL_UNSIGNED_BYTE;
    void* m_data = nullptr;
};
}// namespace HUH
#endif//TEXTURE_H
