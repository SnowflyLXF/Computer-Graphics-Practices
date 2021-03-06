#include "Tutorial.h"
#include "Utils.h"
#include "point2.h"
#include "Game.h"
#include "Colors.h"

void Tutorial::render(const Game &game) const {

    if (game.getGameSeconds() > tutorialEndDate + animationInterval ||
            game.getCurrentState() == GameState::GameOver ||
            game.getCurrentState() == GameState::Paused
            ) return;

    // Setup delle matrici

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glPushMatrix();
    {
        utils::setCoordToPixel(game.screenSize);

        {
            {

                const float animationBeginDate = tutorialBeginDate;
                const float dateNow = game.getGameSeconds();

                const float scaleRaw = (dateNow - animationBeginDate) / animationInterval; 
                const float scaleClamped = fminf(fmaxf(scaleRaw, 0.f), 1.f); 
                const float scaleSin = sinf(scaleClamped * (float)M_PI / 2.f);
                glTranslatef(0.f, game.screenSize.Y()/2.f, 0.f);
                glScalef(1.f, scaleSin, 1.f);
                glTranslatef(0.f, -game.screenSize.Y()/2.f, 0.f);

            }

            {
                const float animationBeginDate = tutorialEndDate;
                const float dateNow =game.getGameSeconds();

                const float scaleRaw = 1 - (dateNow - animationBeginDate) / animationInterval; 
                const float scaleClamped = fminf(fmaxf(scaleRaw, 0.f), 1.f); 
                const float scaleSin = sinf(scaleClamped * (float)M_PI / 2.f);
                glTranslatef(0.f, game.screenSize.Y()/2.f, 0.f);
                glScalef(1.f, scaleSin, 1.f);
                glTranslatef(0.f, -game.screenSize.Y()/2.f, 0.f);
            }
        }

        glPushMatrix();
        {

            const Point2 screenSizeCorrection = game.screenSize / game.defaultScreenSize;
            glScalef(screenSizeCorrection.X(), screenSizeCorrection.Y(), 1.f);


            glPushMatrix();
            {

                const Point2 portraitPosition = Point2{75, 375};
                const Point2 portraitSize = Point2{150.f, 150.f};
                glTranslatef(portraitPosition.X(), portraitPosition.Y(), 0.f);
                glScalef(portraitSize.X(), portraitSize.Y(), 1.f);
                glColor3f(1.f, 1.f, 1.f);
                {
                    glBindTexture(GL_TEXTURE_2D, Texture::PersonalPicture);
                    glEnable(GL_TEXTURE_2D);

                    glBegin(GL_QUADS);
                    {
                        glTexCoord2f(0.0f, 0.0f);
                        glVertex2f(0.f, 0.f);
                        glTexCoord2f(0.0f, 1.0f);
                        glVertex2f(0.f, 1.f);
                        glTexCoord2f(1.0f, 1.0f);
                        glVertex2f(1.f, 1.f);
                        glTexCoord2f(1.0f, 0.0f);
                        glVertex2f(1.f, 0.f);
                    }
                    glEnd();

                    glDisable(GL_TEXTURE_2D);
                }


                glFinish();
            }
            glPopMatrix();

            // Draw Title
            {
                std::string titleString = "Undersea Adventure";

                Point2 titleStringPosition = {game.defaultScreenSize.X() / 2.f - 200.f,
                                              game.defaultScreenSize.Y() / 2.f + 200.f};
                game.textRenderer.render(Colors::White(), Colors::Black(),
                                         titleString.c_str(), titleStringPosition,
                                         blended, big, /*wrapping*/ false);
            }

            // Draw Explanation
            {
                std::string explainString = "Patrick Star lives in the underwater city of Bikini Bottom, and he enjoys collecting jellyfish! Could you help him? Use W A S D to direct the Patrick, Q and E to go up and down, Space to accelerate (once in 5s), F1 to menu. Be aware of SHARKS! Enjoy your undersea adventure now!";

                Point2 explainStringPosition = {game.defaultScreenSize.X() / 2.f - 100.f,
                                                game.defaultScreenSize.Y() / 2.f - 75.f};
                game.textRenderer.render(Colors::White(), Colors::Black(),
                                         explainString.c_str(), explainStringPosition,
                                         blended, small, /*wrapping*/ true);
            }
        }
        glPopMatrix();

    }
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}
