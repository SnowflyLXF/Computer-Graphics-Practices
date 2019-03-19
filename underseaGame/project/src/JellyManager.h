#pragma once

#include "Jellyfish.h"
#include <vector>

class Game;
class Patrick;
class Jellyfish;

class JellyManager {
public:
    void renderJelly(const Game& game) const;
    void doStep(Game &game);
	std::vector<Point3> getJellyPositions(const Game& game) const;
private:
    std::vector<Jellyfish> allJelly{};
    static constexpr float spawnHeight = 50.f;
    float nextSpawnDate = 3.f;
	const float spawnInterval = 3.f;

    void handleJellySpawning(const Game& game);
    void handleCollisions(Game &game);
    bool doesCollide(const Jellyfish& pieceOfJelly, const Game& game) const;
    void spawnPieceOfJelly(const Game& game); };

