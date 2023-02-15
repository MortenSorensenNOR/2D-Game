#ifndef LEVEL_H
#define LEVEL_H

class FloorTile
{
private:
	olc::vf2d pos;
	olc::vi2d size;
	olc::Sprite* sprite;
	olc::vi2d spriteOffset;
	bool flipped = false;

public:
	RenderComponent* renderComponent;

	FloorTile(olc::vf2d _Pos, olc::vi2d _Size, olc::Sprite* _Sprite, olc::vi2d _SpriteOffset)
	{
		pos = _Pos;
		size = _Size;
		sprite = _Sprite;
		spriteOffset = _SpriteOffset;
		renderComponent = new RenderComponent(&pos, &size, &spriteOffset, sprite, &flipped);
	}

	void Render(Camera* camera)
	{
		camera->RenderGameComponent(renderComponent, true);
	}
};

class WallTile
{
private:
	olc::vf2d pos;
	olc::vi2d size;
	olc::Sprite* sprite;
	olc::vi2d spriteOffset;
	bool flipped = false;

public:
	RenderComponent* renderComponent;

	WallTile(olc::vf2d _Pos, olc::vi2d _Size, olc::Sprite* _Sprite, olc::vi2d _SpriteOffset)
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

class Level
{
private:
	vector<FloorTile*> level_floortiles;
	vector<WallTile*> level_walltiles;

public:
	Level(int horizontalTileNum, int verticalTileNum, olc::Sprite* groundSprite, olc::Sprite* wallSprite)
	{
		for (int i = 0; i < verticalTileNum; i++)
		{
			for (int j = 0; j < horizontalTileNum; j++)
			{
				level_floortiles.push_back(new FloorTile(olc::vf2d(j, i) * olc::vf2d(16, 16), olc::vi2d(16, 16), groundSprite, olc::vi2d((std::rand() % 4) * 16, 0)));
			}
		}

		level_walltiles.push_back(new WallTile(olc::vf2d(16, 0), olc::vi2d(16, 16), wallSprite, olc::vi2d(0, 4 * 16)));
		for (int i = 1; i < verticalTileNum - 1; i++)
		{
			level_walltiles.push_back(new WallTile(olc::vf2d(16, i * 16), olc::vi2d(16, 16), wallSprite, olc::vi2d(0, 5 * 16)));
		}
		level_walltiles.push_back(new WallTile(olc::vf2d(16, (verticalTileNum - 1) * 16), olc::vi2d(16, 16), wallSprite, olc::vi2d(0, 6 * 16)));
	}

	void Render(Camera* camera)
	{
		for (int i = 0; i < level_floortiles.size(); i++)
		{
			level_floortiles[i]->Render(camera);
		}

		for (int i = 0; i < level_walltiles.size(); i++)
		{
			level_walltiles[i]->Render(camera);
		}
	}
};

#endif // !LEVEL_H
