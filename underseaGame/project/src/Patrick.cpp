#include <math.h>

#include <GL/gl.h>

#include <vector> 

#include "Patrick.h"
#include "mesh.h"
#include "Textures.h"
#include "Utils.h"
#include "Game.h"

static TriangleWinding patrickMeshWinding = TriangleWinding::CCW;

Mesh patrick("../assets/obj/Patrick.obj", patrickMeshWinding);


extern bool useWireframe; 

void Patrick::DoStep(Game &game) {
    float vxm, vym, vzm; 
    float cosf = cos(facing * (float) M_PI / 180.f);
    float sinf = sin(facing * (float) M_PI / 180.f);
    vxm = +cosf * vx - sinf * vz;
    vym = vy;
    vzm = +sinf * vx + cosf * vz;

    if (game.inputManager.isLeftKeyPressed()) vel += deltavel;
    if (game.inputManager.isRightKeyPressed()) vel -= deltavel;
    vel *= velscale; 

    if (game.inputManager.isMoveForwardPressed()) vzm -= accMax; 
    if (game.inputManager.isMoveBackwardPressed()) vzm += accMax; 

    if (game.inputManager.isUpKeyPressed()) vym += accMax; 
    if (game.inputManager.isDownKeyPressed()) vym -= accMax; 

    if (turbo) {
        if (game.getGameSeconds() < startTurboTime + turboDuration) {
            vzm -= accMax * turboAcc;
        } else {
            turbo = false;
            nextTurbo = game.getGameSeconds() + turboDelay;
        }
    }

    vxm *= frictoX;
    vym *= frictoY;
    vzm *= frictoZ;

    facing = facing - (vzm * grip) * vel;

    float da; //delta angle
    da = (360.f * vzm) / (2.f * (float) M_PI * radiusA);
    angleA += da;
    da = (360.f * vzm) / (2.f * (float) M_PI * radiusP);
    angleP += da;

    vx = +cosf * vxm + sinf * vzm;
    vy = vym;
    vz = -sinf * vxm + cosf * vzm;
        px += vx;
        if (px < minX) { px = minX; }
        if (px > maxX) { px = maxX; }
        py += vy;
        if (py < minY) { py = minY; }
        if (py > maxY) { py = maxY; }
        pz += vz;
        if (pz < minZ) { pz = minZ; }
        if (pz > maxZ) { pz = maxZ; }

}

void Patrick::Init() {
    px = pz = facing = 0.f; 
    py = minY;

    angleA = angleP = vel = 0.f; 
    vx = vy = vz = 0.f;


    deltavel = 2.4f;         
    velscale = 0.93f; 

    accMax = 0.0011f;

    frictoZ = 0.991f;  
    frictoX = 0.8f;  
    frictoY = 0.95f; 


    radiusA = 0.25f;
    radiusP = 0.35f;

    grip = 0.45f; 
}


void Patrick::SetLight() const {

        GLenum usedLight = GL_LIGHT1;
        glEnable(usedLight);

        float position[4] = {0.f, 2.f, 0.f, 1.f};
        glLightfv(usedLight, GL_POSITION, position);

        float direction[3] = {0.f, 0.f, -1.f};
        glLightfv(usedLight, GL_SPOT_DIRECTION, direction);

        glLightf(usedLight, GL_SPOT_CUTOFF, 40.f);
        glLightf(usedLight, GL_SPOT_EXPONENT, 10.f);

        float ambient[4] = {0.0f, 0.0f, 0.0f, 1.f};
        glLightfv(usedLight, GL_AMBIENT, ambient);

        float diffuse[4] = {1.f, 1.f, 1.f, 1.f};
        glLightfv(usedLight, GL_DIFFUSE, diffuse);

        float specular[4] = {0.f, 0.f, 0.f, 1.f};
        glLightfv(usedLight, GL_SPECULAR, specular);

        glLightf(usedLight, GL_CONSTANT_ATTENUATION, 0.f);
        glLightf(usedLight, GL_LINEAR_ATTENUATION, 0.0014f);
        glLightf(usedLight, GL_QUADRATIC_ATTENUATION, 0.007f);


	utils::checkGLError(__FILE__, __LINE__);
}


void Patrick::RenderAllParts(bool usecolor) const {
    glPushMatrix();
    {
        static constexpr float patrickScale = 0.006f;
        const Vector3 ObjectScale{patrickScale, patrickScale, -patrickScale};
        glScalef(ObjectScale.X(), ObjectScale.Y(), ObjectScale.Z());
        static constexpr bool useTexCoords = true;
		utils::checkGLError(__FILE__, __LINE__);
        if (usecolor) {
			glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
			{
				float specular[4] = {1.f, 1.f, 1.f, 1.f};
				glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
				float diffuse[4] = {1.f, 1.f, 1.f, 1.f};
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
				float ambient[4] = {0.0f, 0.0f, 0.0f, 1.f};
				glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
				glMaterialf(GL_FRONT, GL_SHININESS, 32.f);
			}
			utils::checkGLError(__FILE__, __LINE__);
			glColor3f(1.f, 1.f, 1.f);

        	glEnable(GL_LIGHTING);
			glScalef(45, 45, 45);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Texture::Patrick);
			patrick.RenderNxV(useTexCoords); 
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);

			utils::checkGLError(__FILE__, __LINE__);
		} else {
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			patrick.RenderNxV(useTexCoords);
		}
		utils::checkGLError(__FILE__, __LINE__);
    }
    glPopMatrix();
}

void Patrick::render(const Game &game) const {

    glPushMatrix();
    {
        glTranslatef(px, py, pz);
        glRotatef(facing, 0.f, 1.f, 0.f);

        SetLight();
        if (!useHeadlight) {glDisable(GL_LIGHT1);}
        RenderAllParts(/*usecolor*/ true);

    }
    glPopMatrix();

	// Render Shadow
	if (game.useShadow) {
        glPushMatrix();
        {
			glTranslatef(px, 0.35f, pz);
			glRotatef(facing, 0.f, 1.f, 0.f);
            glScalef(1-(getPosition().Y()/40.f), 1- (getPosition().Y()/40.f), 1-(getPosition().Y()/40.f));
			glColor3f(12.f/255.f, 12.f/255.f, 12.f/255.f); 
			glScalef(1.01f, 0.f, 1.01f); 
			glTranslatef(0.f, -pz + 5.f, 0.f); 
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			RenderAllParts(/*usecolor*/ false); 
			glDisable(GL_BLEND);
			glColor3f(1.f, 1.f,1.f);
        }
        glPopMatrix();
    }



    utils::checkGLError(__FILE__, __LINE__);
}

void Patrick::onInputEvent(InputEvent inputEvent, Game& game) {
    switch (inputEvent){

        case InputEvent::TurboButtonPressed:
                if (game.getGameSeconds() > nextTurbo) {
                    turbo = true;
                    startTurboTime = game.getGameSeconds();
                }
            break;

        case InputEvent::SwitchCamera:
            game.switchCamera();
            break;
        case InputEvent::UseHeadlight:
            useHeadlight = !useHeadlight;
            break;

        case InputEvent::ToggleHelpMenu:
            if (game.getGameSeconds() > game.tutorial.tutorialEndDate) {
                game.menuManager.setMenuState(MenuState::graphicsMenu);
                game.setState(GameState::Paused);
            }
            break;

        case InputEvent::ToggleCaustics:
            game.map.caustics = !game.map.caustics;
            break;

        case InputEvent::UseShadows:
            game.useShadow = !game.useShadow;
            break;
    }
}

    float Patrick::getTurboCharge(const Game& game)const {
    float turboCharge = 1.f;
    const float dateNow = game.getGameSeconds();
    const float turboEndDate = startTurboTime + turboDuration;
    if (turbo) {
        turboCharge = 0.f;
    } else if (dateNow >= nextTurbo) {
        turboCharge = 1.f;
    } else {
        float rawTurboCharge = (dateNow - turboEndDate)* (1/(turboDelay));
        turboCharge = fminf(fmaxf(rawTurboCharge, 0.f), 1.f); 
    }
    return turboCharge;
}
