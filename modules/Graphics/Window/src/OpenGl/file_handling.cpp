#include "HUH/Graphics/OpenGl/file_handling.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "SDL3_image/SDL_image.h"
namespace HUH::FileHandling {

SDL_Surface* LoadImage(const char* file) {
    SDL_Surface* LoadedImg = IMG_Load(file);

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    SDL_PixelFormat format = SDL_PIXELFORMAT_ABGR8888;
#else
    SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA8888;
#endif

    SDL_Surface* NLoadedImg = SDL_ConvertSurface(LoadedImg, format);
    SDL_DestroySurface(LoadedImg);
    return NLoadedImg;
}
GLuint LoadShader(GLenum shaderType, const char* filename) {
    std::string shaderCode;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(filename);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderCode = shaderStream.str();
    } catch (const std::ifstream::failure& e) {
        std::cout << "Error" << std::endl;
    }
    const char* shaderCodeCStr = shaderCode.c_str();
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCodeCStr, NULL);
    glCompileShader(shader);
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        std::cout << errorLog.data() << std::endl;

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(shader);// Don't leak the shader.
        return shader;
    }
    return shader;
}
SDL_Surface* LoadImageFromMemory(void* pointer, int size) {
    SDL_IOStream* rwops = SDL_IOFromConstMem(pointer, size);

    SDL_Surface* LoadedImg = IMG_Load_IO(rwops, 0);

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    SDL_PixelFormat format = SDL_PIXELFORMAT_ABGR8888;
#else
    SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA8888;
#endif

    SDL_Surface* image;
    image = SDL_ConvertSurface(LoadedImg, format);

    SDL_DestroySurface(LoadedImg);
    return image;
}
}// namespace HUH::FileHandling
