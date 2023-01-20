#ifndef LEVEL_H
#define LEVEL_H

struct FloorTile
{
	olc::vi2d pos;
	olc::Sprite* sprite;
	FloorTile(olc::Sprite* floorTileSprite, olc::vi2d tilePosition)
	{
		sprite = floorTileSprite;
		pos = tilePosition;
	}

	void render(olc::TransformedView& tv)
	{
		tv.DrawSprite(pos, sprite);
	}
};

struct WallTile
{

};

class Level
{
private:
	olc::vi2d tileSize;
	olc::Sprite* floorTileSprite;
	vector<FloorTile*> levelFloorTiles;

public:
	Level(int verticalTileNum, int horizontalTileNum, olc::Sprite* floorSprite, olc::vi2d& spriteSize)
	{
		tileSize = spriteSize;
		floorTileSprite = floorSprite;
		for (int y = 0; y < verticalTileNum; y++)
		{
			for (int x = 0; x < horizontalTileNum; x++)
			{
				FloorTile* tile = new FloorTile(floorTileSprite, olc::vi2d(x, y) * tileSize);
				levelFloorTiles.push_back(tile);
			}
		}
	}

	void render(olc::TransformedView& tv)
	{
		for (auto floorTile : levelFloorTiles)
		{
			floorTile->render(tv);
		}
	}
};

#endif // !LEVEL_H