#include <map>
#include <list>
#include <queue>
#include <vector>
#include <string>
using namespace std;

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"

// For loading sprites into the game
#include "Assets_Loader.h"

// For drawing and sorting the drawing of sprites
#include "Camera.h"

// For handling animations
#include "Animator.h"

// Main character class - TODO: Replace with specific character classes
#include "Character.h"

// Game Item class
#include "GameItem.h"

// Level class
#include "Level.h"

class Game : public olc::PixelGameEngine
{
public:
	Game() {
		sAppName = "2D Game";
	}

private:
	// Screenspace variables
	int verticalTileNum;
	int horizontalTileNum;
	olc::vi2d default_tile_size = { 16, 16 };

	// Sprites
	AssetsLoader* assetsLoader;

	// Game Elements
	Camera* camera;
	Character* player;

	//FloorTile* grassTile01;
	Level* testLevel;

	olc::vf2d setCameraOffset = { 0.0f, 0.0f };

public:
	bool OnUserCreate() override
	{
		// Initilize screen
		verticalTileNum = ScreenHeight() / default_tile_size.y;
		horizontalTileNum = ScreenWidth() / default_tile_size.x;

		// Load sprites
		SetPixelMode(olc::Pixel::ALPHA);
		assetsLoader = new AssetsLoader();

		// Initilize Game Objects
		player = new Character(olc::vf2d(64, 64), assetsLoader->character_player, { 15, 23 }, character_player_frame_numbers, 4, true);

		// Initilize camera
		camera = new Camera(&setCameraOffset, olc::vi2d(ScreenWidth(), ScreenHeight()));
		setCameraOffset = { float(ScreenWidth()) / 2.0f, float(ScreenHeight()) / 2.0f};

		testLevel = new Level(horizontalTileNum, verticalTileNum, assetsLoader);

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		// ======= User Input =======
		if (GetKey(olc::Key::ESCAPE).bPressed) return false;
		if (GetKey(olc::Key::E).bPressed)
		{
			testLevel->Interact(olc::Key::E, player->pos);
		}
		if (GetKey(olc::Key::W).bPressed || GetKey(olc::Key::W).bHeld || GetKey(olc::Key::UP).bPressed || GetKey(olc::Key::UP).bHeld)
		{
			// UP
			player->dir += olc::vf2d(0.0f, -1.0f);
		}
		if (GetKey(olc::Key::S).bPressed || GetKey(olc::Key::S).bHeld || GetKey(olc::Key::DOWN).bPressed || GetKey(olc::Key::DOWN).bHeld)
		{
			// DOWN
			player->dir += olc::vf2d(0.0f, 1.0f);
		}
		if (GetKey(olc::Key::A).bPressed || GetKey(olc::Key::A).bHeld || GetKey(olc::Key::LEFT).bPressed || GetKey(olc::Key::LEFT).bHeld)
		{
			// LEFT
			player->dir += olc::vf2d(-1.0f, 0.0f);
		}
		if (GetKey(olc::Key::D).bPressed || GetKey(olc::Key::D).bHeld || GetKey(olc::Key::RIGHT).bPressed || GetKey(olc::Key::RIGHT).bHeld)
		{
			// RIGHT
			player->dir += olc::vf2d(1.0f, 0.0f);
		}

		// ======= Update =======
		player->Update(deltaTime);
		testLevel->Update(deltaTime);

		// =======  Draw  =======
		Clear(olc::Pixel(25, 25, 25));
		
		// Render player
		player->Render(camera); 
		
		// Render world
		testLevel->Render(camera);

		// Render all
		camera->Render(this);
		
		return true;
	}
};

int main()
{
	Game game;
	if (game.Construct(416, 320, 1, 1, true, false)) // 640 x 480 or  420 x 320
		game.Start();

	return 0;
}