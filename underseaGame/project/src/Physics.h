#pragma once
#include <SDL2/SDL_hints.h>

namespace Physics {
    constexpr Uint32 stepMs = 10; 
    constexpr float stepSeconds = stepMs / 1000.f;

    constexpr Uint32 fpsSampling = 3000; 
};
