#pragma once

#include "JellyManager.h"
#include "EnemyManager.h"
#include "TextRenderer.h"
#include "Minimap.h"
#include "Patrick.h"
#include "MenuManager.h"
#include "Minimap.h"
#include "Map.h"
#include "Input.h"
#include "GameState.h"
#include "Tutorial.h"

namespace CameraState {
    enum Type {
        CAMERA_BACK_PAT,
        CAMERA_TOP_FIXED,
        CAMERA_LATERAL_PAT,
        CAMERA_TOP_PAT,
        CAMERA_MOUSE,
        CAMERA_MAX
    };
}

class Game {
public:

    Patrick pat;
    JellyManager jellyManager;
    EnemyManager enemyManager;
    MenuManager menuManager;
    TextRenderer textRenderer;
    Map map;
    Minimap minimap;
    Input inputManager;
    Tutorial tutorial;

	static Point2 defaultScreenSize;
	Point2 screenSize;

    Game();

    int getScore() const {
        return score;
    }

    CameraState::Type getCameraState() const {
        return cameraState;
    }

    // field of view
    float viewAlpha = 20.f;
    float viewBeta = 40.f;
    float eyeDist = 5.f; 

    void switchCamera();

    void init();

    void doStep();

    GameState getCurrentState() const;

    void render();

    void setState(const GameState newState);

    int incrementScore(int inc);

    void EatKey(int keycode, bool pressed, bool joystick);

    void EatJoyAxis(int axisIndex, int axisValue);

	float getGameSeconds() const {return gameSeconds;}

    bool useShadow = true;

private:
    static constexpr float gameDuration = 90.f; // in seconds
	float previousElapsedSeconds = 0.f;
	float gameSeconds = 0.f;

    void setCamera() const;

    // CAMERA SETTING
    CameraState::Type cameraState = CameraState::CAMERA_BACK_PAT;



    int score = 0;
    GameState gameState;

};