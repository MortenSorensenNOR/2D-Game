#ifndef GAME_ITEM_H
#define GAME_ITEM_H

class GameItem
{
private:
	olc::vf2d pos;
	olc::vi2d size;
	olc::Sprite* sprite;
	olc::vi2d spriteOffset;
	bool flipped = false;

public:
	RenderComponent* renderComponent;

	GameItem(olc::vf2d _Pos, olc::vi2d _Size, olc::Sprite* _Sprite, olc::vi2d _SpriteOffset)
	{
		pos = _Pos;
		size = _Size;
		sprite = _Sprite;
		spriteOffset = _SpriteOffset;
		renderComponent = new RenderComponent(&pos, &size, &spriteOffset, sprite, &flipped);
	}

	void Render(Camera* camera)
	{
		camera->RenderGameComponent(renderComponent);
	}
};

#endif // !GAME_ITEM_H