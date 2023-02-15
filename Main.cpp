#include <map>
#include <queue>
#include <vector>
#include <string>
using namespace std;

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_SPLASHSCREEN
#include "olcPGEX_SplashScreen.h"

// For loading sprites into the game
#include "Sprite_loader.h"

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
	SpriteLoader* spriteLoader;

	// Game Elements
	Camera* camera;
	Character* player;

	//FloorTile* grassTile01;
	Level* testLevel;
	GameItem* chest01;
	GameItem* tree01;
	GameItem* sign01;

	olc::vf2d setCameraOffset = { 0.0f, 0.0f };

public:
	bool OnUserCreate() override
	{
		// Initilize screen
		verticalTileNum = ScreenHeight() / default_tile_size.y;
		horizontalTileNum = ScreenWidth() / default_tile_size.x;

		// Load sprites
		SetPixelMode(olc::Pixel::ALPHA);
		spriteLoader = new SpriteLoader();

		// Initilize Game Objects
		player = new Character(olc::vf2d(0, 0), spriteLoader->character_player, { 15, 23 }, character_player_frame_numbers, 4, true);

		// Initilize camera
		camera = new Camera(&setCameraOffset, olc::vi2d(ScreenWidth(), ScreenHeight()));
		setCameraOffset = { float(ScreenWidth()) / 2.0f, float(ScreenHeight()) / 2.0f};

		testLevel = new Level(horizontalTileNum, verticalTileNum, spriteLoader->floor_grass_01, spriteLoader->wall_plains);
		chest01 = new GameItem(olc::vf2d(128, 128), olc::vi2d(16, 16), spriteLoader->gameItem_chest, olc::vi2d(0, 0));
		tree01 = new GameItem(olc::vf2d(240, 64), olc::vi2d(46, 64), spriteLoader->gameItem_objects, olc::vi2d(0, 80));
		sign01 = new GameItem(olc::vi2d(12 * 16, 9 * 16), olc::vi2d(16, 16), spriteLoader->gameItem_objects, olc::vi2d(0, 0));

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		// ======= User Input =======
		if (GetKey(olc::Key::ESCAPE).bPressed) return false;
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

		// =======  Draw  =======
		Clear(olc::Pixel(25, 25, 25));
		
		// Render player
		player->Render(camera); 
		
		// Render world
		//grassTile01->Render(camera);
		testLevel->Render(camera);
		chest01->Render(camera);
		tree01->Render(camera);
		sign01->Render(camera);

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