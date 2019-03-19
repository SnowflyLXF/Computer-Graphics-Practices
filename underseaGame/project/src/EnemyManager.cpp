#include "EnemyManager.h"
#include "Utils.h"
#include "MenuManager.h"
#include "Game.h"

Mesh shark((char *) "../assets/obj/shark.obj");
Mesh jellyfish((char *) "../assets/obj/jellyfish.obj");

void EnemyManager::renderEnemies(const Game& game) const {
    for (const Enemy &enemy : allEnemies) {
        enemy.render(game);
    }
	utils::checkGLError(__FILE__, __LINE__);
}

void EnemyManager::doStep(Game &game) {
    for (Enemy &enemy : allEnemies) {
        enemy.doStep(game.pat);
    }
    handleEnemySpawning(game);
    handleCollisions(game.pat, game);
}

void EnemyManager::spawnEnemy(const Game& game) {
    /* generate */
    {
		Point3 spawnPosition;
        if (getRandomSpawnPosition(game.pat.getPosition(), /*out*/ spawnPosition))
        {
          allEnemies.emplace_back(spawnPosition,
                                  1.f,                  // speed
                                  0.005f,                // scale
                                  Point3{0.f, 0.f, 0.f}, // rotation
                                  true,                  // rotatesTowardsPlayer
                                  false,                 // isAnimated
                                  &shark,                // enemyMesh
                                  Texture::Shark);
        }
    }
}

std::vector<Point3> EnemyManager::getEnemyPositions() const
{
	std::vector<Point3> enemyPositions{};
	for (const Enemy& enemy : allEnemies)
	{
		enemyPositions.push_back(enemy.getPosition());
	}
	return enemyPositions;
}

void EnemyManager::handleCollisions(const Patrick &pat, Game &game) {
    auto positions = getEnemyPositions();
    Point3 patrickPosition = pat.getPosition();
    for (const Point3& enemyPosition : positions ){
        if ( utils::distance(enemyPosition, patrickPosition) < pat.getRadius() ) {
            game.setState(GameState::GameOver);
        }
    }


}

void EnemyManager::handleEnemySpawning(const Game &game) {
    float secondsNow = game.getGameSeconds();
    if (secondsNow > nextSpawnDate) {
        if (allEnemies.size() < maxEnemyCount) {
            spawnEnemy(game);
        }
        nextSpawnDate += getNextSpawnInterval();
        ++spawnWave;
    }
}

    float EnemyManager::getNextSpawnInterval() const {
        return baseSpawnInterval * powf(1.f - spawnIntervalReductionFactor, (float) spawnWave);
    }

bool EnemyManager::getRandomSpawnPosition(const Point3& patPosition, Point3& outSpawnPosition) const
{
	static constexpr int maxAttempts = 100;
	for (int loopGuard = 0; loopGuard < maxAttempts; loopGuard++)
	{
		const Point3 randomPosition = Point3{
			utils::random_float() * Map::width - Map::width / 2.f,
			utils::random_float() *	100.f, // It should not spawn below ground level
			utils::random_float() * Map::width - Map::width / 2.f
		};

		if (utils::distance(randomPosition, patPosition) > minSpawnDistanceToPlayer)
		{
			outSpawnPosition = randomPosition;
			return true; // Found a spawn position
		}
	}
	return false; // Failed to find a spawn position
}
