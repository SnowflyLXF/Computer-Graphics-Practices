#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>
#include "TextRenderer.h"
#include "Textures.h"
#include "Utils.h"

void TextRenderer::initialize() {
    // font
    smallFont = TTF_OpenFont("../assets/font/Oswald-Bold.ttf", 25);
    if (smallFont == NULL) {
        fprintf(stderr, "Impossibile caricare il font (piccolo).\n");
    };
    bigFont = TTF_OpenFont("../assets/font/Oswald-Bold.ttf", 40);
    if (bigFont == NULL) {
        fprintf(stderr, "Impossibile caricare il font (grande).\n");
    };


#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

}

void TextRenderer::render(SDL_Color textColor, SDL_Color backgroundColor, const char *text, Point2 position, enum textquality quality,
                          enum fontSize fontSize, bool wrap) const {
	glFrontFace(GL_CCW);
    SDL_Surface *textSurface;
    SDL_Surface *surfaceRGB;
    SDL_Rect location;
    int w, h;

    textSurface = NULL;
    if (fontSize == small) {
        if (quality == solid)
            textSurface = TTF_RenderText_Solid(smallFont, text, textColor);
        else if (quality == shaded)
            textSurface = TTF_RenderText_Shaded(smallFont, text, textColor, backgroundColor);
        else if (quality == blended && wrap)
            textSurface = TTF_RenderText_Blended_Wrapped(smallFont, text, textColor, 450);
        else if (quality == blended && !wrap)
            textSurface = TTF_RenderText_Blended(smallFont, text, textColor);
    } else if (fontSize == big){
        if (quality == solid)
            textSurface = TTF_RenderText_Solid(bigFont, text, textColor);
        else if (quality == shaded)
            textSurface = TTF_RenderText_Shaded(bigFont, text, textColor, backgroundColor);
        else if (quality == blended && wrap)
            textSurface = TTF_RenderText_Blended_Wrapped(bigFont, text, textColor, 400);
        else if (quality == blended && !wrap)
            textSurface = TTF_RenderText_Blended(bigFont, text, textColor);
    }

	assert(textSurface);

    w = textSurface->w;
    h = textSurface->h;

    surfaceRGB = SDL_CreateRGBSurface(0, w, h, 32,
                                      rmask, gmask, bmask, amask);

    if (surfaceRGB == NULL) {
        SDL_Log("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
        exit(1);
    }

    SDL_BlitSurface(textSurface, NULL, surfaceRGB, 0);

    glBindTexture(GL_TEXTURE_2D, Texture::Font);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, surfaceRGB->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    location.x = (int) position.X();
    location.y = (int) position.Y();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Texture::Font);
    glEnable(GL_BLEND);


    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f((float)location.x, (float)location.y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f((float)location.x + w, (float)location.y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f((float)location.x + w, (float)location.y + h);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f((float)location.x, (float)location.y + h);
    glEnd();

    glFinish();
    location.w = textSurface->w;
    location.h = textSurface->h;

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    SDL_FreeSurface(textSurface);
    SDL_FreeSurface(surfaceRGB);
	utils::checkGLError(__FILE__, __LINE__);

	glFrontFace(GL_CW);

}


