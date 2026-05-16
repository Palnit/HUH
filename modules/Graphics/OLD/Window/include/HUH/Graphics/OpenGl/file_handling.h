#ifndef GPGPU_EDGE_DETECTOR_INCLUDE_GENERAL_F_H_
#define GPGPU_EDGE_DETECTOR_INCLUDE_GENERAL_F_H_

#include "GL/glew.h"
#include "HUH/definitions.h"
#include "SDL3/SDL_surface.h"

/*!
 * Namespace containing useful functions for file handling
 */
namespace HUH::FileHandling {
/*!
 * Loads any kinds of image in RGBA format into an SDL_Surface object
 * \param file The path to the image file to be opened
 * \return A pointer to the constructed SDL_Surface object the user is responsible
 * for the pointers lifetime
 */
HUH_GRAPHICS_API SDL_Surface*  LoadImage(const char* file);

/*!
 * Loads any kind of shader from a shader file and compiles it
 * \param shaderType The type of shader to be compiled into
 * \param filename The path to the shader file
 * \return The OpenGl identifier for the shader
 */
HUH_GRAPHICS_API GLuint LoadShader(GLenum shaderType, const char* filename);

/// Function to load data from memory
/// @param pointer pointer to memory
/// @param size size of memory
/// @return the returned sdl_surface pointer
HUH_GRAPHICS_API SDL_Surface* LoadImageFromMemory(void* pointer, int size);

} // FileHandling

#endif //GPGPU_EDGE_DETECTOR_INCLUDE_GENERAL_F_H_
