#pragma once

#include "point2.h"
#include "point3.h"
#include <vector>

class Game;
class Point3;


struct Rock
{
	Point3 position;
	Vector3 scale;
	Vector3 rotation;

	Rock(const Point3& Position, const Vector3& Scale, const Vector3& Rotation)
		: position{Position},
		  scale{Scale},
		  rotation{Rotation}
	{
	}
};


class Map{
public:
	void initialize();
	void render(const Game& game) const; 
    static constexpr float width = 100.f; 
	static constexpr float terrainHeight = 0.f;
	static constexpr float terrainWidth = 400.f; 
	static constexpr float waterHeight = 40.f;
	static constexpr float waterPanSpeed = 0.05f;
	static constexpr int starCount = 4;
	static constexpr int coralCount =20;
	bool caustics = true;
    std::vector<Rock> stars;
	std::vector<Rock> corals;
private:
    void drawSand(const Point3& patPosition) const;
	void drawWaterBackground() const;
	void drawPlane(const float width, const int quadCount, const float tileSize, const Point3& center = Point3{0.f, 0.f, 0.f}) const;
	void drawCaustics(const Game& game) const;
	void drawRocks() const;
	void drawCorals() const;


	
};
