#ifndef CHARACTER_H
#define CHARACTER_H

class Character
{
public:
	// Movement
	olc::vf2d pos;
	olc::vf2d dir;
	float walkSpeed = 150;

	// Character traits
	bool friendly = true;

	// Sprite Setup
	olc::Sprite* sprite;
	olc::vi2d spriteSize;
	olc::vi2d spriteOffset = { 0, 0 };
	RenderComponent* renderComponent;

public:
	Character(olc::vf2d _Pos, olc::Sprite* _Sprite, olc::vi2d _SpriteSize, bool _Friendly)
	{
		// Movement
		pos = _Pos;
		
		// Traits
		friendly = _Friendly;

		// Sprite
		sprite = _Sprite;
		spriteSize = _SpriteSize;
		renderComponent = new RenderComponent(&pos, &spriteSize, spriteOffset, sprite, false);
	}

	void update(float dt)
	{
		if (abs(dir.x) || abs(dir.y))
		{
			dir = dir.norm();
			pos.x = pos.x + 100 * dir.x * dt;
			pos.y = pos.y + 100 * dir.y* dt;
		}
		dir = { 0.0f, 0.0f };
	}
};

#endif // !CHARACTER_H