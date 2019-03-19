#include "Enemy.h"
#include "Physics.h"
#include "Utils.h"
#include "Game.h"

Enemy::Enemy(const Point3 &position, float speed, float scale,
             const Vector3 &rotation, bool rotatesTowardsPlayer,
             bool isAnimated, const Mesh *const enemyMesh, GLuint texture)
        : position(position), speed(speed),
          rotatesTowardsPlayer(rotatesTowardsPlayer), isAnimated(isAnimated),
          enemyMesh(enemyMesh), scale(scale), textureID(texture) {}

void Enemy::render(const Game &game) const {
    glPushMatrix();
    {
        glEnable(GL_LIGHTING);
        glTranslate(position);
        if (isAnimated) {
            glScalef(1.f, (cosf(game.getGameSeconds()) + 4.f) / 4.f, 1.f); // movimento ondulatorio sull'asse Y per le meduse
        }
        glScalef(scale*1.5f, scale*1.5f, scale*1.5f);
        glRotatef(rotation.X(), 1.f, 0.f, 0.f); // rotazione intorno all'asse X
        glRotatef(rotation.Y(), 0.f, 1.f, 0.f); // rotazione intorno all'asse Y
        glRotatef(rotation.Z(), 0.f, 0.f, 1.f); // rotazione intorno all'asse Z

        // activate the texture

        glBindTexture(GL_TEXTURE_2D, textureID);
        glEnable(GL_TEXTURE_2D);
        assert(enemyMesh);
        // Mesh rendering
        enemyMesh->RenderNxV(/*useTextureCoords*/ true);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
    }
    glPopMatrix();

    // Render Shadow
    if (game.useShadow) {
        glPushMatrix();
        {
            glTranslatef(position.X(), 0.35f, position.Z()); // Segue il nemico ma rimanendo al livello del terreno sull'asse delle Y
            /* The scale is multiplied by a parameter that depends on the position of the real enemy, 
            to make it closer to the ground the more the shadow is enlarged */
            glScalef(scale * (1 - (position.Y() / 40.f)),
                     scale * (1 - position.Y() / 40.f),
                     scale * (1 - (position.Y() / 40.f)));

            // flatten on the Y, I enlarge of 1% on the X and the Z
            glScalef(1.01f, 0.f, 1.01f);

            glRotatef(rotation.X(), 1.f, 0.f, 0.f); 
            glRotatef(rotation.Y(), 0.f, 1.f, 0.f); 
            glRotatef(rotation.Z(), 0.f, 0.f, 1.f); 

            glColor3f(12.f / 255.f, 12.f / 255.f, 12.f / 255.f); // shadow color

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            enemyMesh->RenderNxV(/*useTextureCoords*/ false);

            glDisable(GL_BLEND);
            glColor3f(1.f, 1.f, 1.f);
        }
        glPopMatrix();
    }
    utils::checkGLError(__FILE__, __LINE__);
}

void Enemy::doStep(const Patrick &pat) {

    /* CALCULATE NEW POSITION */
    {
        const Point3 patCoord = pat.getPosition();
        // Calculate the vector from the enemy to the patrick
        const Vector3 vectorToPat = patCoord - position;
        const float patToEnemyDistance = vectorToPat.modulo();  
        // Calculate the distance that the enemy travels in a physics step
        // Checking that you do not go too far
        const float distanceToTravel =
                fminf(speed * Physics::stepSeconds, patToEnemyDistance);
        // Calculate the vector that describes the movement of the enemy in one step
        const Vector3 travelVector = vectorToPat.NormalizeSafe() * distanceToTravel;
        position = position + travelVector;
    }
    /* CALCULATE NEW ROTATION */
    if (rotatesTowardsPlayer) {
        const Vector3 vectorToPat = pat.getPosition() - position;
        /* use atan2f to calculate the angle with the Y axis*/
        const float facingRadiansY = atan2f(vectorToPat.X(), vectorToPat.Z());
        const float facingDegreesY =
                utils::radiansToDegrees(facingRadiansY); // openGL usa i gradi
        rotation = Vector3{0.f, facingDegreesY, 0.f};
    }
}
