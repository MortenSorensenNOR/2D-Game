#ifndef GAME_ITEM_H
#define GAME_ITEM_H

class GameItem
{
public:
	// Intrinsics
	int id;
	int state;
	olc::vf2d pos;

	// Rendering
	olc::Sprite* sprite;
	olc::vi2d spriteSize;
	olc::vi2d spriteOffset;
	bool flipped = false;

	RenderComponent* renderComponent;

	GameItem(olc::vf2d _Pos, olc::vi2d _SpriteSize, olc::Sprite* _Sprite, olc::vi2d _SpriteOffset, int _Id)
	{
		// Intrinsics
		id = _Id;
		state = 0;
		pos = _Pos;

		// Rendering
		spriteSize = _SpriteSize;
		sprite = _Sprite;
		spriteOffset = _SpriteOffset;

		renderComponent = new RenderComponent(&pos, &spriteSize, &spriteOffset, sprite, &flipped);
	}

	virtual void Update(float dt) {}

	void Render(Camera* camera)
	{
		camera->RenderGameComponent(renderComponent);
	}
};

// ===== Chest =====
enum GameItem_Chest_States
{
	CLOSED,
	OPENING,
	OPEN,
	END = OPEN
};

class Chest : public GameItem
{
public:	
	// Animation for open
	float animationTime = 0.0f;
	float animationFrame = 0;
	float animationTimeBetweenFrames = 1.0f / 8.0f;

	Chest(olc::vf2d _Pos, olc::vi2d _SpriteSize, olc::Sprite* _Sprite, olc::vi2d _SpriteOffset, int _Id) : GameItem(_Pos, _SpriteSize, _Sprite, _SpriteOffset, _Id) {}

	void Update(float dt)
	{
		if (state == GameItem_Chest_States::OPENING)
		{
			animationTime += dt;
			if (animationTime > animationTimeBetweenFrames)
			{
				animationTime -= animationTimeBetweenFrames;
				if (animationFrame >= 3)
				{
					state = GameItem_Chest_States::OPEN;
				}
				else
				{
					animationFrame++;
					spriteOffset.x += spriteSize.x;
				}
			}
		}
	}

};

#endif // !GAME_ITEM_H