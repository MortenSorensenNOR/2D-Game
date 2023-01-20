#include <vector>
#include <cstdlib>
using namespace std;

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_TRANSFORMEDVIEW
#include "olcPGEX_TransformedView.h"

#include "GameObject.h"
#include "Character.h"
#include "Player.h"
#include "State.h"
#include "Level.h"
#include "GUI.h"

class Game : public olc::PixelGameEngine
{
public:
	Game() {
		sAppName = "2D Game";
	}

	~Game()
	{
		free(character);
		free(player);
	}

protected:
	olc::TransformedView tv;

private:
	int verticalTileNum;
	int horizontalTileNum;
	olc::vi2d tileSize = { 16, 16 };

	// Sprites
	olc::Sprite* character;
	olc::Sprite* floorTile;

	// Game Elements
	Level* newLevel;
	Character* player;

	// Game State
	KeyBoardState keyboardState;


public:
	bool OnUserCreate() override
	{
		// Initilize screen
		// TODO: Sette initial zoom for å fikse at den er veldig zoomet ut
		tv.Initialise({ ScreenWidth(), ScreenHeight() });

		verticalTileNum = ScreenHeight() / tileSize.y;
		horizontalTileNum = ScreenWidth() / tileSize.x;

		// Load sprites
		SetPixelMode(olc::Pixel::MASK);
		floorTile = new olc::Sprite("./gfx/Sprite_pack/tilesets/floors/wooden.png");
		character = new olc::Sprite("./gfx/Sprite_pack/characters/player.png");

		// Initilize Game Objects
		newLevel = new Level(verticalTileNum, horizontalTileNum, floorTile, tileSize);
		player = new Character(olc::vf2d(50.0f, 50.0f), character, true, &keyboardState);

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		// User Input
		if (GetKey(olc::Key::ESCAPE).bPressed) return false;
	
		// Pan and zoom
		tv.HandlePanAndZoom();

		// Update
		for (int i = 0; i < olc::Key::ENUM_END; i++)
		{
			if (GetKey(olc::Key(i)).bPressed || GetKey(olc::Key(i)).bHeld)
			{
				keyboardState.keyStates[i] = true;
			}
			else
			{
				keyboardState.keyStates[i] = false;
			}
		}

		player->update(deltaTime);

		// Draw
		Clear(olc::BLACK);
		newLevel->render(tv);
		player->render(tv);
		
		return true;
	}
};

int main()
{
	Game game;
	if (game.Construct(640, 480, 1, 1, true, true))
		game.Start();

	return 0;
}