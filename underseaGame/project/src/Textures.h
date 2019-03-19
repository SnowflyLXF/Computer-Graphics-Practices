#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace Texture
{
    extern GLuint Font;
	extern GLuint WaterBackground;
	extern GLuint Barrels;
	extern GLuint Jellyfish;
	extern GLuint Sand;
	extern GLuint Patrick;
	extern GLuint Purple;
	extern GLuint Pink;
	extern GLuint Yellow;
	extern GLuint Orange;	
    extern GLuint Shark;
    extern GLuint Sonar;
	extern GLuint Caustics;
	extern GLuint PersonalPicture;
	extern GLuint Seaweed;
	extern GLuint Rock;

	bool Load(/*out*/ GLuint &textureID, const char *filename); 
	bool LoadAllTextures(); 
}

