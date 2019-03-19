#pragma once

#include "point3.h"
#include "Patrick.h"
#include "mesh.h"
#include "Textures.h"
#include <memory>

class Game;

class Enemy {
public:
    Enemy(const Point3 &position, float speed, float scale, const Vector3 &rotation, bool rotatesTowardsPlayer,
          bool isAnimated,
          const Mesh *enemyMesh, GLuint textureName);

    Point3 getPosition() const {
        return position;
    }

    float getRadius() const {
        return 0.5f;
    }

    void render(const Game &game) const;

    void doStep(const Patrick &pat);

    Vector3 rotation; // in degree

private:
    Point3 position;
    float speed; // in meters per second
    bool rotatesTowardsPlayer;// true if the enemy is facing the player
    bool isAnimated; 
    const Mesh *enemyMesh; 
    float scale; // size of enemy
    GLuint textureID;

};
