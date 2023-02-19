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
	olc::Decal* decal;
	olc::vi2d decalSize;
	olc::vi2d decalOffset;
	bool flipped = false;

	RenderComponent* renderComponent;

	GameItem(olc::vf2d _Pos, olc::vi2d _DecalSize, olc::Decal* _Decal, olc::vi2d _DecalOffset, int _Shadow_offset, int _Id)
	{
		// Intrinsics
		id = _Id;
		state = 0;
		pos = _Pos;

		// Rendering
		decal = _Decal;
		decalSize = _DecalSize;
		decalOffset = _DecalOffset;

		renderComponent = new RenderComponent(&pos, &decalSize, &decalOffset, decal, &flipped, _Shadow_offset);
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

	Chest(olc::vf2d _Pos, olc::vi2d _DecalSize, olc::Decal* _Decal, olc::vi2d _DecalOffset, int _Shadow_offset, int _Id) : GameItem(_Pos, _DecalSize, _Decal, _DecalOffset, _Shadow_offset, _Id) {}

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
					decalOffset.x += decalSize.x;
				}
			}
		}
	}

};

#endif // !GAME_ITEM_H