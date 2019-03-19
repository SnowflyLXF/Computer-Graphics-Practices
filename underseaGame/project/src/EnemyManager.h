#pragma once

#include <vector>
#include <SDL2/SDL_ttf.h>
#include "Enemy.h"


class Game;
class Patrick;
class MenuManager;
class Point3;

class EnemyManager {
public:
    void renderEnemies(const Game& game) const;
    void doStep(Game &game);
	std::vector<Point3> getEnemyPositions() const;
private:
    std::vector<Enemy> allEnemies;
    static constexpr int maxEnemyCount = 3;
	static constexpr float minSpawnDistanceToPlayer = 20.f;
	float nextSpawnDate = 3.f;
	int spawnWave = 0;
	static constexpr float  spawnIntervalReductionFactor = 0.005f;
	static constexpr float  baseSpawnInterval = 3.f;
	void handleCollisions(const Patrick &pat, Game &game);
	void handleEnemySpawning(const Game& game);
	void spawnEnemy(const Game& game);
	float getNextSpawnInterval() const;
	// Returns success
	bool getRandomSpawnPosition(const Point3& patPosition, Point3& outSpawnRadius) const;

};
