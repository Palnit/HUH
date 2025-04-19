#include <HUH/Graphics/file_handling.h>

#include <SDL3_image/SDL_image.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
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
    return shader;
}
}
