#include <GL/glu.h>
#include "Game.h"
#include "Utils.h"
#include "Colors.h"

Game::Game() {}

Point2 Game::defaultScreenSize = Point2{750.f, 750.f};

int Game::incrementScore(int inc) {
    return score += inc;
}

void Game::init() {
	textRenderer.initialize();
	map.initialize();
    setState(GameState::Playing);
    screenSize  = defaultScreenSize;
}

void Game::doStep() {
	const float deltaSeconds = utils::get_elapsed_seconds() - previousElapsedSeconds;
	previousElapsedSeconds = utils::get_elapsed_seconds();
	if (gameState == GameState::Playing) {
		gameSeconds += deltaSeconds;
		pat.DoStep(*this);
		jellyManager.doStep(*this);
		enemyManager.doStep(*this);
	}
}

GameState Game::getCurrentState() const {
    return gameState;
}


void Game::setState(const GameState newState) {
    if (gameState == newState) return;
    gameState = newState;
    switch (gameState) {
        case GameState::GameOver: {
            menuManager.setMenuState(MenuState::gameOver);
            break;
        }

    }


}

void Game::render() {

            glLineWidth(3.f); 
            glViewport(0, 0,  (GLsizei)screenSize.X(),  (GLsizei)screenSize.Y());
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(70, 
                           screenSize.X() / screenSize.Y(),//aspect Y/X,
                           0.2,  //distance of NEAR CLIPPING PLANE in coordinates view
                           1000  //distance of FAR CLIPPING PLANE in view coordinates
                            );
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            setCamera();

			GLfloat lightColor[4] = {11.f / 255.f, 70.f / 255.f, 107.f / 255.f, 1.0f};

			// Lighting
			{
				float direction[4] = {0.0f, 1.f, 0.5f, 0.f}; 
				glLightfv(GL_LIGHT0, GL_POSITION, direction);

				float ambient[4] = {0.0f, 0.0f, 0.0f, 1.f};
				glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
				glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor );
				float specular[4] = {0.f, 0.f, 0.f, 1.f};
				glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
				glEnable(GL_LIGHT0);
				glEnable(GL_LIGHTING);
			}


            // Enable fog drawing
            {
                GLfloat* fogColor = lightColor;
                glClearColor(fogColor[0], fogColor[1], fogColor[2], fogColor[3]); // Set the background to the fog color
                glFogi(GL_FOG_MODE, GL_EXP);
                glFogfv(GL_FOG_COLOR, fogColor);

                glFogf(GL_FOG_DENSITY, 0.05f);        // For exponential fog

                glHint(GL_FOG_HINT, GL_NICEST);

                // For linear fog
                glFogf(GL_FOG_START, 10.0f);
                glFogf(GL_FOG_END, 200.0f);

                glEnable(GL_FOG);
            }

            map.render(*this);
            pat.render(*this);
            jellyManager.renderJelly(*this);
            enemyManager.renderEnemies(*this);

            glFinish();

            glDisable(GL_DEPTH_TEST);
            glDisable(GL_LIGHTING);

            // move on to the 2D drawing
            utils::setCoordToPixel(screenSize);

            const Point2 turboIndicatorSize{120.f, 35.f};
            const Point2 bottomLeftCorner{screenSize.X() - (turboIndicatorSize.X() + /* offset */ 30 ) , screenSize.Y() - ( turboIndicatorSize.Y() + /* offset */ 30 )};


            if (pat.getTurboCharge(*this) == 0) {
                Point2 textPosition = bottomLeftCorner;
                textPosition.X() = textPosition.X() - 60;
		textPosition.Y() = textPosition.Y() - 60;
                textRenderer.render(Colors::Red(),Colors::Black(),"TURBO!", textPosition, blended, big, false);
            }
			glFrontFace(GL_CCW);
            glEnable(GL_SCISSOR_TEST);

            {
              glDisable(GL_TEXTURE_2D);

              glBegin(GL_TRIANGLES);
              {
                glColor3f(1.f, 0.f, 0.f);
                glVertex2f(bottomLeftCorner.X(), bottomLeftCorner.Y());
                glColor3f(0.f, 1.f, 0.f);
                glVertex2f(bottomLeftCorner.X() + turboIndicatorSize.X(),
                           bottomLeftCorner.Y() + 0.f);
                glColor3f(0.f, 1.f, 0.f);
                glVertex2f(bottomLeftCorner.X() + turboIndicatorSize.X(),
                           bottomLeftCorner.Y() + turboIndicatorSize.Y());
              }
              glEnd();
              glColor3f(1.f, 1.f, 1.f);
            }

            glDisable(GL_SCISSOR_TEST);
			glFrontFace(GL_CW);

            minimap.render(*this);


    {
        // score
        std::stringstream scoreStream;
        scoreStream << "Points: " << getScore();
        Point2 textPosition = {(float) 30, (screenSize.Y() - 50)};
        std::string scoreString = scoreStream.str();
        textRenderer.render(Colors::White(), Colors::Black(), scoreString.c_str(), textPosition, blended, small,
                            false);
    }
    {
        // time
        std::stringstream timeStream;
        std::stringstream timeLeftStream;
        float gameSecondsNow = getGameSeconds();
        int timeLeft = (int)gameDuration - (int)gameSecondsNow + 9; // 9s duration for tutorial
        if (timeLeft == 0) {
            setState(GameState::GameOver);
        }
        if (timeLeft < 60) {
            if (timeLeft < 10){
                timeLeftStream << "00:0" << timeLeft;
            } else {
                timeLeftStream << "00:" << timeLeft;
            }
        } else {
            if (timeLeft-60 < 10) {
                timeLeftStream << "01:0" << timeLeft - 60;
            } else {
                timeLeftStream << "01:" << timeLeft - 60;
            }
        }

        Point2 textPosition = {(screenSize.Y() - 150), (screenSize.Y() - 50)};
        std::string timeString = timeLeftStream.str();
        if (gameSecondsNow >= 9) {
            textRenderer.render(Colors::White(), Colors::Black(), timeString.c_str(), textPosition, blended, small,
                                false);
        }
    }
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_LIGHTING);

    switch (gameState) {

        case GameState::GameOver : {
            menuManager.render(*this);
        }

        case GameState::Paused : {
            menuManager.render(*this);
        }

    }
    glColor3f(1.f, 1.f, 1.f);

    tutorial.render(*this);

    glFinish();
    utils::checkGLError(__FILE__, __LINE__);
}

void Game::setCamera() const {

    double px = pat.getPosition().X();
    double py = pat.getPosition().Y();
    double pz = pat.getPosition().Z();
    double angle = pat.getFacing();
    double cosf = cos(angle * M_PI / 180.0);
    double sinf = sin(angle * M_PI / 180.0);
    double camd, camh, ex, ey, ez, cx, cy, cz;
    double cosff, sinff;

// check the position of the camera according to the selected option
    switch (cameraState) {
        case CameraState::CAMERA_BACK_PAT:
            camd = 2.5;
            camh = 1.0;
            ex = px + camd * sinf;
            ey = py + camh;
            ez = pz + camd * cosf;
            cx = px - camd * sinf;
            cy = py + camh;
            cz = pz - camd * cosf;
            gluLookAt(ex, ey, ez, cx, cy, cz, 0.0, 1.0, 0.0);
            break;
        case CameraState::CAMERA_LATERAL_PAT:
            camd = 6.0;
            camh = 1.15;
            angle = pat.getFacing() - 40.0;
            cosff = cos(angle*M_PI/180.0);
            sinff = sin(angle*M_PI/180.0);
            ex = px + camd*sinff;
            ey = py + camh;
            ez = pz + camd*cosff;
            cx = px - camd*sinf;
            cy = py + camh;
            cz = pz - camd*cosf;
            gluLookAt(ex,ey,ez,cx,cy,cz,0.0,1.0,0.0);
            break;
        case CameraState::CAMERA_TOP_FIXED:
            camd = 2.5;
            camh = 1.0;
            ex = px + camd * sinf;
            ey = py + camh;
            ez = pz + camd * cosf;
            cx = px - camd * sinf;
            cy = py + camh;
            cz = pz - camd * cosf;
            gluLookAt(ex, ey + 5, ez, cx, cy, cz, 0.0, 1.0, 0.0);
            break;
        case CameraState::CAMERA_TOP_PAT:
            camd = 0.5;
            camh = 0.55;
            cosff = cos(angle * M_PI / 180.0);
            sinff = sin(angle * M_PI / 180.0);
            ex = px + camd * sinff;
            ey = py + camh;
            ez = pz + camd * cosff;
            cx = px - camd * sinf;
            cy = py + camh;
            cz = pz - camd * cosf;
            gluLookAt(ex, ey, ez, cx, cy, cz, 0.0, 1.0, 0.0);
            break;
        case CameraState::CAMERA_MOUSE:
            glTranslatef(0, 0, -eyeDist);
            glRotatef(viewBeta, 1, 0, 0);
            glRotatef(viewAlpha, 0, 1, 0);
            break;
    }
}

void Game::switchCamera() {
    cameraState = (CameraState::Type) (((int) cameraState + 1) % (int) CameraState::CAMERA_MAX);
}

void Game::EatKey(int keycode, bool pressed, bool joystick) {
    inputManager.EatKey(keycode, pressed, joystick, *this);
}

void Game::EatJoyAxis(int axisIndex, int axisValue) {
    inputManager.EatJoyAxis(axisIndex, axisValue);
}




