#include <queue>
#include <vector>
using namespace std;

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "Camera.h"
#include "Character.h"

class Game : public olc::PixelGameEngine
{
public:
	Game() {
		sAppName = "2D Game";
	}

	~Game()
	{
		free(woodenFloorTile);
		free(chestSprite);
		free(character);
		free(grassTile);
	}

private:
	int verticalTileNum;
	int horizontalTileNum;
	olc::vi2d tileSize = { 32, 32 };

	// Sprites
	olc::Sprite* woodenFloorTile;
	olc::Sprite* chestSprite;
	olc::Sprite* character;
	olc::Sprite* grassTile;

	// Game Elements
	Camera* camera;
	Character* player;

	olc::vf2d pos1 = { 0, 0 };
	olc::vf2d pos2 = { 0, 0 };
	olc::vi2d size = { 16, 16 };
	olc::vi2d offset = { 0, 0 };

	RenderComponent* block1;
	RenderComponent* block2;

public:
	bool OnUserCreate() override
	{
		// Initilize screen
		verticalTileNum = ScreenHeight() / tileSize.y;
		horizontalTileNum = ScreenWidth() / tileSize.x;

		// Load sprites
		SetPixelMode(olc::Pixel::MASK);
		woodenFloorTile = new olc::Sprite("./gfx/Sprite_pack/tilesets/floors/wooden.png");
		grassTile = new olc::Sprite("./gfx/Sprite_pack/tilesets/decor_16x16.png");
		character = new olc::Sprite("./gfx/Sprite_pack/characters/player.png");
		chestSprite = new olc::Sprite("./gfx/Sprite_pack/objects/chest_01.png");

		// Initilize Game Objects
		player = new Character(olc::vf2d(0.0f, 0.0f), character, { 48, 48 }, true);

		// Initilize camera
		camera = new Camera(&player->pos, olc::vi2d(ScreenWidth(), ScreenHeight()));

		block1 = new RenderComponent(&pos1, &size, offset, grassTile, false);
		block2 = new RenderComponent(&pos2, &size, offset, chestSprite, false);

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