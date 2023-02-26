#ifndef LEVEL_H
#define LEVEL_H

class FloorTile
{
private:
	olc::vf2d pos;
	olc::vi2d size;
	olc::Decal* decal;
	olc::vi2d decalOffset;
	bool flipped = false;

public:
	RenderComponent* renderComponent;

	FloorTile(olc::vf2d _Pos, olc::vi2d _Size, olc::Decal* _Decal, olc::vi2d _DecalOffset)
	{
		pos = _Pos;
		size = _Size;
		decal = _Decal;
		decalOffset = _DecalOffset;
		renderComponent = new RenderComponent(&pos, &size, &decalOffset, decal, &flipped, 0.0f);
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
	olc::Decal* decal;
	olc::vi2d decalOffset;
	bool flipped = false;

public:
	RenderComponent* renderComponent;

	WallTile(olc::vf2d _Pos, olc::vi2d _Size, olc::Decal* _Decal, olc::vi2d _DecalOffset)
	{
		pos = _Pos;
		size = _Size;
		decal = _Decal;
		decalOffset = _DecalOffset;
		renderComponent = new RenderComponent(&pos, &size, &decalOffset, decal, &flipped, 0.0f);
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
	vector<GameElement*> level_game_elements;

public:
	Level(int horizontalTileNum, int verticalTileNum, SpriteLoader* spriteLoader)
	{
		// Floor
		for (int i = 0; i < verticalTileNum; i++)
		{
			for (int j = 0; j < horizontalTileNum; j++)
			{
				level_floortiles.push_back(new FloorTile(olc::vf2d(j, i) * olc::vf2d(16, 16), olc::vi2d(16, 16), spriteLoader->decal_floor_grass_01, olc::vi2d((std::rand() % 4) * 16, 0)));
			}
		}

		// Walls
		level_walltiles.push_back(new WallTile(olc::vf2d(16, 0), olc::vi2d(16, 16), spriteLoader->decal_wall_plains, olc::vi2d(0, 4 * 16)));
		for (int i = 1; i < verticalTileNum - 1; i++)
		{
			level_walltiles.push_back(new WallTile(olc::vf2d(16, i * 16), olc::vi2d(16, 16), spriteLoader->decal_wall_plains, olc::vi2d(0, 5 * 16)));
		}
		level_walltiles.push_back(new WallTile(olc::vf2d(16, (verticalTileNum - 1) * 16), olc::vi2d(16, 16), spriteLoader->decal_wall_plains, olc::vi2d(0, 6 * 16)));

		// Game items
		level_game_elements.push_back(new GameElement(olc::vf2d(9 * 16, 9 * 16), olc::vi2d(16, 16), olc::vi2d(16, 16), spriteLoader->decal_gameItem_chest, olc::vi2d(0, 0), 2, 1)); // Chest
		level_game_elements.push_back(new GameElement(olc::vf2d(13 * 16, 96), olc::vi2d(46, 64), olc::vi2d(46, 64), spriteLoader->decal_gameItem_objects, olc::vi2d(0, 80), 6, 2)); // Tree
		level_game_elements.push_back(new GameElement(olc::vi2d(10 * 16, 9 * 16), olc::vi2d(16, 16), olc::vi2d(16, 16), spriteLoader->decal_gameItem_objects, olc::vi2d(0, 0), 3, 3)); // Sign
		level_game_elements.push_back(new GameElement(olc::vi2d(11 * 16, 9 * 16), olc::vi2d(16, 16), olc::vi2d(16, 16), spriteLoader->decal_gameItem_objects, olc::vi2d(0, 16), 1, 4)); // Rock
		level_game_elements.push_back(new GameElement(olc::vi2d(12 * 16, 9 * 16), olc::vi2d(16, 16), olc::vi2d(16, 16), spriteLoader->decal_gameItem_objects, olc::vi2d(80, 0), 1, 5)); // Crate
		level_game_elements.push_back(new GameElement(olc::vi2d(13 * 16 + 48, 128), olc::vi2d(32, 32), olc::vi2d(32, 32), spriteLoader->decal_gameItem_objects, olc::vi2d(96, 112), 6, 6)); // Bush
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

	void Render(Camera* camera)
	{
		for (int i = 0; i < level_floortiles.size(); i++)
		{
			level_floortiles[i]->Render(camera);
		}

		//for (int i = 0; i < level_walltiles.size(); i++)
		//{
		//	level_walltiles[i]->Render(camera);
		//}

		for (int i = 0; i < level_game_elements.size(); i++)
		{
			level_game_elements[i]->Render(camera);
		}
	}
};

#endif // !LEVEL_H