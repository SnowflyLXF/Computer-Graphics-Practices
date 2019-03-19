#include "Textures.h"
#include "Patrick.h"
#include "JellyManager.h"
#include "EnemyManager.h"
#include "Physics.h"
#include "Minimap.h"
#include "Utils.h"
#include "MenuManager.h"
#include "Game.h"

Uint32 nstep = 0; // number of PHYSICS steps made so far

float fps = 0; 
int fpsNow = 0; 
Uint32 timeLastInterval = 0;

static Game game;

void rendering(SDL_Window *win) {

    // extra framme!
    fpsNow++;
    game.render();
    glFinish();
    utils::checkGLError(__FILE__, __LINE__);
    SDL_GL_SwapWindow(win);
}

void redraw() {
    SDL_Event e;
    e.type = SDL_WINDOWEVENT;
    e.window.event = SDL_WINDOWEVENT_EXPOSED;
    SDL_PushEvent(&e);
}

int main(int argc, char *argv[]) {

    SDL_Window *win;
    SDL_GLContext mainContext;
    Uint32 windowID;
    SDL_Joystick *joystick;

    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

    // Initialize SDL_TTF
    if (TTF_Init() < 0) {
        fprintf(stderr, "Impossibile inizializzare TTF: %s\n", SDL_GetError());
        SDL_Quit();
        return (2);
    }


    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Creo una finestra
    win = SDL_CreateWindow(argv[0], 0, 0, (int) Game::defaultScreenSize.X(), (int) Game::defaultScreenSize.Y(),
                           SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    //Create our OpengGL context and attach it to our window
    mainContext = SDL_GL_CreateContext(win);

    // Smoothing modes (
    {
        glEnable(GL_POLYGON_SMOOTH);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_POINT_SMOOTH);
        glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    }

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE); 
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW); 
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_POLYGON_OFFSET_FILL); 
    glPolygonOffset(1.f, 1.f);             

    utils::checkGLError(__FILE__, __LINE__);

    if (!Texture::LoadAllTextures()) return -1;

    game.init();


    bool done = false;
    while (!done) {

        SDL_Event e;

        if (SDL_PollEvent(&e)) {
            // if yes, process event
            switch (e.type) {
                case SDL_KEYDOWN:
                    game.EatKey(e.key.keysym.sym, true, false);
                    break;
                case SDL_KEYUP:
                    game.EatKey(e.key.keysym.sym, false, false);
                    break;
                case SDL_QUIT:
                    done = true;
                    break;
                case SDL_WINDOWEVENT:
                    if (e.window.event == SDL_WINDOWEVENT_EXPOSED)
                        rendering(win);
                    else {
                        windowID = SDL_GetWindowID(win);
                        if (e.window.windowID == windowID &&
                            e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                            game.screenSize = Point2{(float) e.window.data1, (float) e.window.data2};
                            rendering(win);
                            redraw(); // richiedi ridisegno
                            break;
                        }
                    }
                    break;

                case SDL_MOUSEMOTION:
                    if ((e.motion.state & SDL_BUTTON(1)) && game.getCameraState() == CameraState::CAMERA_MOUSE) {
                        game.viewAlpha += e.motion.xrel;
                        game.viewBeta += e.motion.yrel;
                        if (game.viewBeta < +5) game.viewBeta = +5; //per non andare sotto la macchina
                        if (game.viewBeta > +90) game.viewBeta = +90;
                    }
                    break;

                case SDL_MOUSEWHEEL:
                    if (game.getCameraState() == CameraState::CAMERA_MOUSE) {
                        if (e.wheel.y > 0) {
                            // zoom in
                            game.eyeDist = game.eyeDist * 0.9f;
                            if (game.eyeDist < 3) game.eyeDist = 3;
                        };
                        if (e.wheel.y < 0) {
                            // zoom out
                            game.eyeDist = game.eyeDist / 0.9f;
                            if(game.eyeDist > 100) game.eyeDist = 100; // impedisco all'utente di uscire dal mondo con la visuale
                        };
                    }
                    break;

                case SDL_JOYAXISMOTION: /* Handle Joystick Motion */
                    game.EatJoyAxis(e.jaxis.axis, e.jaxis.value);
                    break;

                    /* Handle Joystick Button Presses */
                case SDL_JOYBUTTONDOWN:
                    game.EatKey(e.jbutton.button, true, /* joystick */ true);
                    break;

                case SDL_JOYBUTTONUP:
                    game.EatKey(e.jbutton.button, false, /* joystick */ true);
                    break;

            }

        } else {

            Uint32 timeNow = SDL_GetTicks(); 

            if (timeLastInterval + Physics::fpsSampling < timeNow) {
                fps = 1000.f * ((float) fpsNow) / (timeNow - timeLastInterval);
                fpsNow = 0;
                timeLastInterval = timeNow;
            }

            bool doneSomething = false;
            int guardia = 0; 

            while (nstep * Physics::stepMs < timeNow) {
                game.doStep();
                doneSomething = true;
                nstep++;
                timeNow = SDL_GetTicks();
                if (guardia++ > 1000) {
                    done = true;
                    break;
                } 
            }

            if (doneSomething)
                rendering(win);
                //redraw();
            else {

            }
        }

        utils::checkGLError(__FILE__, __LINE__);
    }
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return (0);
}

