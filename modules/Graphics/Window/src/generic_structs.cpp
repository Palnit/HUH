
#include "HUH/Graphics/generic_structs.h"
#include "SDL3/SDL.h"

void HUH::Graphics::ErrorHandling::HandelSDLError(const char* type) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "[%s] Error during the SDL initialization: %s",
                 type,
                 SDL_GetError());
}
