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
	// Level base
	vector<WallTile*> level_walltiles;
	vector<FloorTile*> level_floortiles;

	// Level items
	vector<GameItem*> level_gameitems;

public:
	Level(int horizontalTileNum, int verticalTileNum, AssetsLoader* assetsLoader)
	{
		// Floor
		for (int i = 0; i < verticalTileNum; i++)
		{
			for (int j = 0; j < horizontalTileNum; j++)
			{
				level_floortiles.push_back(new FloorTile(olc::vf2d(j, i) * olc::vf2d(16, 16), olc::vi2d(16, 16), assetsLoader->floor_grass_01, olc::vi2d((std::rand() % 4) * 16, 0)));
			}
		}

		// Walls
		level_walltiles.push_back(new WallTile(olc::vf2d(16, 0), olc::vi2d(16, 16), assetsLoader->wall_plains, olc::vi2d(0, 4 * 16)));
		for (int i = 1; i < verticalTileNum - 1; i++)
		{
			level_walltiles.push_back(new WallTile(olc::vf2d(16, i * 16), olc::vi2d(16, 16), assetsLoader->wall_plains, olc::vi2d(0, 5 * 16)));
		}
		level_walltiles.push_back(new WallTile(olc::vf2d(16, (verticalTileNum - 1) * 16), olc::vi2d(16, 16), assetsLoader->wall_plains, olc::vi2d(0, 6 * 16)));

		// Game items
		level_gameitems.push_back(new Chest(olc::vf2d(128, 128), olc::vi2d(16, 16), assetsLoader->gameItem_chest, olc::vi2d(0, 0), 69));
		level_gameitems.push_back(new GameItem(olc::vf2d(240, 64), olc::vi2d(46, 64), assetsLoader->gameItem_objects, olc::vi2d(0, 80), 1));
		level_gameitems.push_back(new GameItem(olc::vi2d(12 * 16, 9 * 16), olc::vi2d(16, 16), assetsLoader->gameItem_objects, olc::vi2d(0, 0), 2));
	}

	~Level()
	{
		for (FloorTile* tile : level_floortiles)
		{
			free(tile);
		}
		for (WallTile* tile : level_walltiles)
		{
			free(tile);
		}
	}

	void Update(float dt)
	{
		for (GameItem* item : level_gameitems)
		{
			item->Update(dt);
		}
	}

	void Interact(olc::Key key, olc::vf2d& player_position)
	{
		if (key == olc::Key::E)
		{
			if (olc::vf2d(level_gameitems[0]->pos - player_position).mag() < 20)
			{
				if (level_gameitems[0]->state == GameItem_Chest_States::CLOSED)
				{
					level_gameitems[0]->state = GameItem_Chest_States::OPENING;
				}
			}
		}
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

		for (int i = 0; i < level_gameitems.size(); i++)
		{
			level_gameitems[i]->Render(camera);
		}
	}
};

#endif // !LEVEL_H
