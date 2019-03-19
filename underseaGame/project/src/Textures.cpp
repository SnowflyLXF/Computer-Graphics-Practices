#include "Textures.h"

#include <GL/glu.h>
#include "Utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cassert>

GLuint Texture::Font = 0;
GLuint Texture::WaterBackground = 0;
GLuint Texture::Barrels = 0;
GLuint Texture::Jellyfish = 0;
GLuint Texture::Sand = 0;
GLuint Texture::Patrick = 0;
GLuint Texture::Purple = 0;
GLuint Texture::Pink = 0;
GLuint Texture::Orange = 0;
GLuint Texture::Yellow = 0;
GLuint Texture::Shark = 0;
GLuint Texture::Sonar = 0;
GLuint Texture::Caustics = 0;
GLuint Texture::PersonalPicture = 0;
GLuint Texture::Seaweed = 0;
GLuint Texture::Rock = 0;


bool Texture::Load(/*out*/ GLuint &textureID, const char *filename) {
	assert(filename);
	assert(!glIsTexture(textureID));
	SDL_Surface * const s = IMG_Load(filename);
	if (!s) {
        printf("Texture not found: %s", filename);
        return false;
    }
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	gluBuild2DMipmaps(
		GL_TEXTURE_2D,
		GL_RGB,
		s->w, s->h,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		s->pixels);
	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);

	SDL_FreeSurface(s);
	utils::checkGLError(__FILE__, __LINE__);
	return true;
}

bool Texture::LoadAllTextures()
{
	if (!Load(WaterBackground, "../assets/textures/underwater2.jpg")) return false;
	if (!Load(Barrels, "../assets/textures/barrels.jpg")) return false;
	if (!Load(Jellyfish, "../assets/textures/jellyfish.png")) return false;
if (!Load(Patrick, "../assets/textures/pink.png")) return false;
if (!Load(Purple, "../assets/textures/purple.jpg")) return false;
if (!Load(Pink, "../assets/textures/pink.jpg")) return false;
if (!Load(Orange, "../assets/textures/orange.jpg")) return false;
if (!Load(Yellow, "../assets/textures/yellow.jpg")) return false;
    if (!Load(Sand, "../assets/textures/sand.jpg")) return false;
    if (!Load(Shark, "../assets/textures/shark.jpg")) return false;
    if (!Load(Sonar, "../assets/textures/board.jpg")) return false;
    if (!Load(Caustics, "../assets/textures/Caustics1.png")) return false;
	if (!Load(PersonalPicture, "../assets/textures/instructor.jpg")) return false;
    if (!Load(Seaweed, "../assets/textures/seaweed.png")) return false;
	if (!Load(Rock, "../assets/textures/rock.jpg")) return false;

	return true;
}
