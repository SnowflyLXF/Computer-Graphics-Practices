#pragma once

#include "point3.h"
#include "Map.h"
#include "InputEvent.h"

class Game;
class Input;
class Enemy;
enum class InputEvent;


class Patrick {

public:
    void Init(); 
    void render(const Game &game) const; 
    void DoStep(Game &game); 
    Patrick() { Init(); } 

    void onInputEvent(InputEvent inputEvent, Game& game); 

    float getRadius() const {
        return 3.f;
    }

    Point3 getPosition() const {
        return {px, py, pz};
    }

	float getFacing() const { return facing;}

    float getTurboCharge(const Game& game)const;

private:
	float px = 0.f;

	float py, pz, facing; 
	float angleA, angleP, vel; 
	float vx, vy, vz; 

	float deltavel, velscale, accMax,
		radiusA, radiusP, grip,
		frictoX, frictoY, frictoZ; 

    bool useHeadlight = true;

private:
    bool turbo = false; 
    float startTurboTime = 0.f; 
    float nextTurbo = 0.f; 
    float turboDelay = 30.f; 
    float turboDuration = 5.f; 
    float turboAcc = 2.f; 

    float maxX = 50.f;
    float minX = -50.f;
    float maxZ = 50.f;
    float minZ = -50.f;
    float maxY = Map::waterHeight;
    float minY = 1.5f;

    void RenderAllParts(bool usecolor) const;
    void SetLight() const;
};
