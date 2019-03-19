#include "mesh.h"
#include "Jellyfish.h"
#include "Textures.h"
#include "Utils.h"
#include "Game.h"

Mesh barrels((char *) "../assets/obj/jellyfish.obj");

Jellyfish::Jellyfish(const Point3 &initialPosition, const float spawnTime, const float fallSpeed, const Vector3 &rotation)

        : initialPosition(initialPosition), spawnTime(spawnTime), fallSpeed(fallSpeed), rotation(rotation) {}

Point3 Jellyfish::getPosition(const float gameSeconds) const {
    const float lifeTime = gameSeconds - spawnTime;
    return initialPosition + -Vector3::GetUpVector() * fallSpeed * lifeTime;
}

void Jellyfish::render(const float gameSeconds, bool useShadow) const {
	const Point3 currentPosition = getPosition(gameSeconds);
	static constexpr float jellyScale = 1.5f;
    glPushMatrix();
    {
        
        glTranslate(currentPosition);
        glScalef(jellyScale,jellyScale,jellyScale);
        glRotatef(rotation.X(),1.f,0.f,0.f); 
        glRotatef(rotation.Y(),0.f,1.f,0.f); 
        glRotatef(rotation.Z(),0.f,0.f,1.f);


		glBindTexture(GL_TEXTURE_2D, (GLuint)Texture::Jellyfish);
		glEnable(GL_TEXTURE_2D);
	 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glEnable( GL_BLEND );
	glColor4f(1.f, 1.f, 1.f, 0.6f);	
	glEnable(GL_LIGHTING);
		float specular[4] = {0.9f, 0.9f, 0.9f, 1.0f};
    	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    	float diffuse[4] = {0.4f, 0.4f, 0.4f, 1.0f};
    	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
   	float ambient[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	float emission[4]= {0.0f, 0.0f, 0.0f, 0.0f};
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
    	glMaterialf(GL_FRONT, GL_SHININESS, 96.f);
        barrels.RenderNxV(/*usTexCoords*/ false);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
        //glColor4f(1.f,1.f,1.f,0.3f);
    }
    glPopMatrix();
    if (currentPosition.Y() > 0.f) {
        // Render Shadow
        if (useShadow) {
            glPushMatrix();
            {
                glTranslatef(currentPosition.X(), 0.35f, currentPosition.Z());

                glScalef(jellyScale * (1 - (currentPosition.Y() / /* spawnHeight */40.f)),
                         jellyScale * (1 - (currentPosition.Y() / /* spawnHeight */40.f)),
                         jellyScale * (1 - (currentPosition.Y() / /* spawnHeight */40.f)));
                glScalef(1.01f, 0.f, 1.01f);
                glRotatef(rotation.X(), 1.f, 0.f, 0.f); 
                glRotatef(rotation.Y(), 0.f, 1.f, 0.f); 
                glRotatef(rotation.Z(), 0.f, 0.f, 1.f); 
                glColor3f(12.f / 255.f, 12.f / 255.f, 12.f / 255.f); 
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                barrels.RenderNxV(/*usTexCoords*/ false);
                glDisable(GL_BLEND);
                glColor4f(1.f, 1.f, 1.f, 1.f);
            }
            glPopMatrix();
        }
    }
    utils::checkGLError(__FILE__, __LINE__);
}
