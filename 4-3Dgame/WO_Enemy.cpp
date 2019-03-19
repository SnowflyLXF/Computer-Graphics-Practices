#include "WO_Enemy.h"

using namespace std;

/*----------------------------*/
// CONSTRUCTORS AND DESTRUCTORS
/*----------------------------*/
WO_Enemy::WO_Enemy()
{
	world_pos = Vec3D(0, 0, 0);
	world_indices = Coord2D(0,0);
	size = Vec3D(1, 1, 1);
	start_vertex_index = 0;
	total_vertices = 0;
	mat = Material();
	dir = Vec3D(1, 0, 0);
}

WO_Enemy::WO_Enemy(Coord2D indices)
{
	world_pos = Vec3D(0, 0, 0);
	world_indices = indices;
	size = Vec3D(1, 1, 1);
	start_vertex_index = 0;
	total_vertices = 0;
	mat = Material();
	dir = Vec3D(1, 0, 0);
}

WO_Enemy::~WO_Enemy()
{
}

int WO_Enemy::getType()
{
	return ENEMY_WOBJ;
}

Vec3D WO_Enemy::getDir()
{
	return dir;
}

void WO_Enemy::setDir(Vec3D d)
{
	dir = d;
}

bool WO_Enemy::hit(Vec3D player_pos)
{
	if (sqrt((player_pos.getX()-world_pos.getX())*(player_pos.getX()-world_pos.getX()) + (player_pos.getY()-world_pos.getY())*(player_pos.getY()-world_pos.getY()) + (player_pos.getZ()-world_pos.getZ())*(player_pos.getZ()-world_pos.getZ()))<0.25)
		return true;
	else return false;
}

bool WO_Enemy::attack(Vec3D player_pos)
{
	if (sqrt((player_pos.getX()-world_pos.getX())*(player_pos.getX()-world_pos.getX()) + (player_pos.getY()-world_pos.getY())*(player_pos.getY()-world_pos.getY()) + (player_pos.getZ()-world_pos.getZ())*(player_pos.getZ()-world_pos.getZ()))<0.05)
		return true;
	else return false;
}

