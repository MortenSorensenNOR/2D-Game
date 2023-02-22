#ifndef COLLISION_H
#define COLLISION_H

class CollisionObject
{
private:
	olc::vf2d* pos;
	olc::vf2d size;
	olc::vf2d offset;

public:
	CollisionObject(olc::vf2d* _Pos, olc::vf2d _Size, olc::vf2d _Offset)
	{
		pos = _Pos;
		size = _Size;
		offset = _Offset;
	}
};

class CollisionHandler
{
private:
	list<CollisionObject*> collisionObjects;

public:
	CollisionHandler()
	{

	}

	list<CollisionObject*>::iterator addCollisionObject(olc::vf2d* _Pos, olc::vf2d _Size, olc::vf2d _Offset)
	{
		collisionObjects.push_back(new CollisionObject(_Pos, _Size, _Offset));
		return --collisionObjects.end();
	}

	void removeCollisionObject(list<CollisionObject*>::iterator it)
	{
		collisionObjects.erase(it);
	}
};

#endif // !COLLISION_H
