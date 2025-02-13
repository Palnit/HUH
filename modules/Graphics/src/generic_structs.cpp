
#include <SDL2/SDL.h>
#include <HUH/Graphics/generic_structs.h>

void HUH::ErrorHandling::HandelSDLError(const char* type) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "[%s] Error during the SDL initialization: %s",
                 type,
                 SDL_GetError());
}
