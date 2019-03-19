#include "JellyManager.h"
#include "Utils.h"
#include "Map.h"
#include "Game.h"
#include <algorithm>

class Game;

/* Draw all the jellyfishes */
void JellyManager::renderJelly(const Game& game) const {
    for (const Jellyfish &pieceOfJelly : allJelly) {
        pieceOfJelly.render(game.getGameSeconds(), game.useShadow);
    }
	utils::checkGLError(__FILE__, __LINE__);
}

bool JellyManager::doesCollide(const Jellyfish &pieceOfJelly, const Game& game) const {
    const Point3 jellyPosition = pieceOfJelly.getPosition(game.getGameSeconds());
    const Point3 patrickPosition = game.pat.getPosition();
    return utils::distance(jellyPosition, patrickPosition) < pieceOfJelly.getRadius() + game.pat.getRadius();
}

void JellyManager::handleCollisions(Game &game) {
    const float secondsNow = game.getGameSeconds();

    const auto collidesWithTerrain = [&](const Jellyfish &pieceOfJelly) {
        return pieceOfJelly.getPosition(secondsNow).Y() <
               Map::terrainHeight - pieceOfJelly.getRadius();

    };

    // destroy when it reach the ground
    const auto isJellyDestroyed = [&](const Jellyfish &pieceOfJelly) {
        return doesCollide(pieceOfJelly, game) || collidesWithTerrain(pieceOfJelly);
    };

    // update scoores
    for (const Jellyfish& pieceOfJelly : allJelly){
        if (doesCollide(pieceOfJelly, game)) {
            game.incrementScore(10);
        }
    }

    allJelly.erase(
            std::remove_if(
                    allJelly.begin(),
                    allJelly.end(),
                    isJellyDestroyed),
            allJelly.end());

}

void JellyManager::handleJellySpawning(const Game& game) {
    float secondsNow = game.getGameSeconds();
    if (secondsNow > nextSpawnDate) {
        spawnPieceOfJelly(game);
        nextSpawnDate += spawnInterval;
    }
}

void JellyManager::doStep(Game &game) {

    handleJellySpawning(game);
    handleCollisions(game);

}

std::vector<Point3> JellyManager::getJellyPositions(const Game& game) const
{
	std::vector<Point3> jellyPositions{};
	const float elapsedSeconds = game.getGameSeconds();
	for (const Jellyfish& pieceOfJelly : allJelly)
	{
		jellyPositions.push_back(pieceOfJelly.getPosition(elapsedSeconds));
	}
	return jellyPositions;
}

void JellyManager::spawnPieceOfJelly(const Game& game) {
    const Point3 jellyRandomPosition = Point3{utils::random_float() * Map::width - Map::width / 2.f,
                                             spawnHeight,
                                             utils::random_float() * Map::width - Map::width / 2.f
    };
    const Vector3 jellyRandomRotation = Vector3{utils::random_float()*360.f, 
                                               utils::random_float()*360.f,
                                               utils::random_float()*360.f
    };
    allJelly.emplace_back(
            jellyRandomPosition,
            /*spawnTime in seconds*/game.getGameSeconds(),
            /*fallSpeed*/1.f,
            jellyRandomRotation);
}

