#ifndef WO_ENEMY_INCLUDED
#define WO_ENEMY_INCLUDED

#include "WorldObject.h"
class WO_Enemy : public WorldObject
{
public:
	//CONSTRUCTORS AND DESTRUCTORS
	WO_Enemy();
	WO_Enemy(Coord2D indices);
	~WO_Enemy();

    //SETTERS
	void setID(char c);
	void setDir(Vec3D d);

	//GETTERS
	char getID();
	Vec3D getDir();

	//VIRTUAL
	int getType();

	bool hit(Vec3D player_pos);
	bool attack(Vec3D player_pos);

private:
	Vec3D dir;
};

#endif