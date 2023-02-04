#include <map>
#include <queue>
#include <vector>
#include <string>
using namespace std;

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// For loading sprites into the game
#include "Sprite_loader.h"

// For drawing and sorting the drawing of sprites
#include "Camera.h"

// Main character class - TODO: Replace with specific character classes
#include "Character.h"

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

	olc::vf2d pos1 = { 0, 0 };
	olc::vf2d pos2 = { 0, 0 };
	olc::vi2d size = { 16, 16 };
	olc::vi2d offset = { 0, 0 };

	olc::vf2d setCameraOffset = { 0.0f, 0.0f };

	RenderComponent* block1;
	RenderComponent* block2;

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
		player = new Character(olc::vf2d(-75.001f, 0.0f), spriteLoader->character_player, { 15, 21 }, true);

		// Initilize camera
		camera = new Camera(&setCameraOffset, olc::vi2d(ScreenWidth(), ScreenHeight()));

		block1 = new RenderComponent(&pos1, &size, offset, spriteLoader->floor_grass_01, false);
		block2 = new RenderComponent(&pos2, &size, offset, spriteLoader->gameItem_chest, false);

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		// User Input
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

		// Update
		player->update(deltaTime);

		// Draw
		Clear(olc::Pixel(25, 25, 25));
		camera->renderGameComponent(player->renderComponent); // Render player
		camera->renderGameComponent(block1, true);
		camera->renderGameComponent(block2);
		camera->render(this);
		
		return true;
	}
};

int main()
{
	Game game;
	if (game.Construct(420, 320, 1, 1, true, true)) // 640 x 480
		game.Start();

	return 0;
}