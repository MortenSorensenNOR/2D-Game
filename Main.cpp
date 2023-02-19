#include <map>
#include <list>
#include <queue>
#include <vector>
#include <string>
#include <time.h>
#include <stdlib.h>
using namespace std;

// Sound Library
#include <irrKlang.h>
using namespace irrklang;

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

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

enum GameStates
{
	MENUE,
	SETTINGS,
	LOADING_GAME,
	GAME
};

class Game : public olc::PixelGameEngine
{
public:
	Game() {
		sAppName = "2D Game";
	}

private:
	// ======= MENUE =======
	int menue_current_option = 0;
	vector<string> menue_options = { "Play", "Settings", "Quit" };
	
	int menue_font_size = 2;
	int menue_option_offset_top;
	int menue_option_offset_left;
	int menue_option_offset_between;

	// ======= SETTINGS =======
	int settings_current_option = 0;
	vector<string> settings_options = { "Video", "Sound", "Back" };

	int settings_font_size = 2;
	int settings_option_offset_top;
	int settings_option_offset_left;
	int settings_option_offset_between;

	// ======= LOAD GAME  =======
	float load_animation_time = 0.0f;
	float load_max_animation_time = 1.5f;

	// ======= GAME  =======
	float game_fade_in_time = 0.0f;
	float game_max_fade_in_time = 0.5f;

	// Screenspace variables
	int verticalTileNum;
	int horizontalTileNum;
	olc::vi2d default_tile_size = { 16, 16 };

	// Game State
	int game_state = GameStates::MENUE;

	// Sprites
	AssetsLoader* assetsLoader;

	// Camera
	Camera* camera;
	olc::vf2d setCameraOffset = { 0.0f, 0.0f };

	// Game elements
	Level* testLevel;
	Character* player;

	// ======= SOUND =======
	ISoundEngine* soundEngine;
	ISound* background_music;
	ISound* ui_sound;

	ik_f32 sound_music_volume;
	ik_f32 sound_ui_volume;

public:
	bool OnUserCreate() override
	{
		srand(time(NULL));

		// ===== MENUE =====
		menue_option_offset_top = ScreenHeight() / 2 - 35;
		menue_option_offset_left = ScreenWidth() / 2 - 65;
		menue_option_offset_between = ScreenHeight() / 12;

		// ===== SETTINGS =====
		settings_option_offset_top = ScreenHeight() / 2 - 35;
		settings_option_offset_left = ScreenWidth() / 2 - 65;
		settings_option_offset_between = ScreenHeight() / 12;

		// ===== GAME ======
		// Initilize screen
		verticalTileNum = ScreenHeight() / default_tile_size.y;
		horizontalTileNum = ScreenWidth() / default_tile_size.x;

		// Load sprites
		SetPixelMode(olc::Pixel::ALPHA);
		assetsLoader = new AssetsLoader();

		// Initilize Game Objects
		player = new Character(olc::vf2d(64, 64), assetsLoader->sprite_character_player, { 15, 23 }, character_player_frame_numbers, 4, true);

		// Initilize camera
		camera = new Camera(&setCameraOffset, olc::vi2d(ScreenWidth(), ScreenHeight()), true);
		setCameraOffset = { float(ScreenWidth()) / 2.0f, float(ScreenHeight()) / 2.0f};

		testLevel = new Level(horizontalTileNum, verticalTileNum, assetsLoader);
		
		// ===== SOUND =====
		soundEngine = createIrrKlangDevice();
		if (!soundEngine)
			return false;

		background_music = soundEngine->play2D("./sfx/intro_song.wav", true, true);
		if (background_music)
		{
			background_music->setVolume(0.42f);
			background_music->setIsPaused(false);
		}

		return true;
	}

	bool OnUserDestroy() override
	{
		if (background_music)
		{
			background_music->drop();
			background_music = 0;
		}
		soundEngine->drop();
		return true;
	}

	bool MenueLoop(float dt)
	{
		// ======= USER INPUT =======
		if (GetKey(olc::Key::ENTER).bReleased)
		{
			if (menue_current_option == 0)
				game_state = GameStates::LOADING_GAME;
			else if (menue_current_option == 1)
				game_state = GameStates::SETTINGS;
			else if (menue_current_option == 2)
				return false;
		}
		if (GetKey(olc::Key::UP).bPressed || GetKey(olc::Key::W).bPressed)
		{
			menue_current_option = max(0, menue_current_option - 1);
		}
		else if (GetKey(olc::Key::DOWN).bPressed || GetKey(olc::Key::S).bPressed)
		{
			menue_current_option = min(int(menue_options.size() - 1), menue_current_option + 1);
		}
		if (GetKey(olc::Key::ENTER).bPressed)
		{
			soundEngine->play2D("./sfx/ui_click.mp3");
		}

		// ======= RENDERING =======
		Clear(olc::PixelF(0.1, 0.1, 0.1, 1.0f));
		for (int i = 0; i < menue_options.size(); i++)
		{
			if (i == menue_current_option)
			{
				int shadow_offset = (GetKey(olc::Key::ENTER).bPressed || GetKey(olc::Key::ENTER).bHeld) ? 1 : 0;
				DrawString(olc::vi2d(menue_option_offset_left + 1, menue_option_offset_top + i * menue_option_offset_between + 1), menue_options[i] + " <--", olc::GREY, menue_font_size);
				DrawString(olc::vi2d(menue_option_offset_left + shadow_offset, menue_option_offset_top + i * menue_option_offset_between + shadow_offset), menue_options[i] + " <--", olc::WHITE, menue_font_size);
			}
			else
			{
				DrawString(olc::vi2d(menue_option_offset_left, menue_option_offset_top + i * menue_option_offset_between), menue_options[i], olc::WHITE, menue_font_size);
			}
		}

		// Render cursor
		DrawSprite(olc::vf2d(GetMouseX(), GetMouseY()), assetsLoader->cursor);

		return true;
	}

	bool SettingsLoop(float dt)
	{
		if (GetKey(olc::Key::ESCAPE).bPressed)
			game_state = GameStates::MENUE;
		if (GetKey(olc::Key::ENTER).bReleased)
		{
			if (settings_current_option == 2)
				game_state = GameStates::MENUE;
		}
		if (GetKey(olc::Key::UP).bPressed || GetKey(olc::Key::W).bPressed)
		{
			settings_current_option = max(0, settings_current_option - 1);
		}
		else if (GetKey(olc::Key::DOWN).bPressed || GetKey(olc::Key::S).bPressed)
		{
			settings_current_option = min(int(settings_options.size() - 1), settings_current_option + 1);
		}
		if (GetKey(olc::Key::ENTER).bPressed)
		{
			soundEngine->play2D("./sfx/ui_click.mp3");
		}

		// ======= RENDERING =======
		Clear(olc::PixelF(0.1, 0.1, 0.1, 1.0f));
		for (int i = 0; i < settings_options.size(); i++)
		{
			if (i == settings_current_option)
			{
				int shadow_offset = (GetKey(olc::Key::ENTER).bPressed || GetKey(olc::Key::ENTER).bHeld) ? 1 : 0;
				DrawString(olc::vi2d(settings_option_offset_left + 1, settings_option_offset_top + i * settings_option_offset_between + 1), settings_options[i] + " <--", olc::GREY, settings_font_size);
				DrawString(olc::vi2d(settings_option_offset_left + shadow_offset, settings_option_offset_top + i * settings_option_offset_between + shadow_offset), settings_options[i] + " <--", olc::WHITE, settings_font_size);
			}
			else
			{
				DrawString(olc::vi2d(settings_option_offset_left, settings_option_offset_top + i * settings_option_offset_between), settings_options[i], olc::WHITE, settings_font_size);
			}
		}

		// Render cursor
		DrawSprite(olc::vf2d(GetMouseX(), GetMouseY()), assetsLoader->cursor);

		return true;
	}

	bool LoadingGameLoop(float dt)
	{
		load_animation_time += dt;
		if (load_animation_time > load_max_animation_time)
		{
			game_state = GameStates::GAME;
			camera->Init(this);
			return true;
		}

		// ======= RENDERING =======
		float alpha = 1.0f - load_animation_time / load_max_animation_time;
		Clear(olc::PixelF(0.1, 0.1, 0.1, 1.0f));
		for (int i = 0; i < menue_options.size(); i++)
		{
			if (i == menue_current_option)
			{
				DrawString(olc::vi2d(menue_option_offset_left + 1, menue_option_offset_top + i * menue_option_offset_between + 1), menue_options[i] + " <--", olc::PixelF(0.5f, 0.5f, 0.5f, alpha), menue_font_size);
				DrawString(olc::vi2d(menue_option_offset_left, menue_option_offset_top + i * menue_option_offset_between), menue_options[i] + " <--", olc::PixelF(1.0f, 1.0f, 1.0f, alpha), menue_font_size);
			}
			else
			{
				DrawString(olc::vi2d(menue_option_offset_left, menue_option_offset_top + i * menue_option_offset_between), menue_options[i], olc::PixelF(1.0f, 1.0f, 1.0f, alpha), menue_font_size);
			}
		}

		// Render cursor
		DrawSprite(olc::vf2d(GetMouseX(), GetMouseY()), assetsLoader->cursor);

		return true;
	}

	bool GameLoop(float dt)
	{
		// ======= User Input =======
		if (GetKey(olc::Key::ESCAPE).bPressed)
		{
			soundEngine->play2D("./sfx/ui_click.mp3");
			game_state = GameStates::MENUE;
		}
		if (GetKey(olc::Key::E).bPressed)
		{
			soundEngine->play2D("./sfx/coin.mp3");
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
		player->Update(dt);
		testLevel->Update(dt);

		// =======  Draw  =======
		Clear(olc::PixelF(0.1, 0.1, 0.1, 1.0f));

		// Render player
		player->Render(camera);

		// Render world
		testLevel->Render(camera);

		// Render all
		camera->Render(this, assetsLoader);

		// Render cursor
		DrawSprite(olc::vf2d(GetMouseX(), GetMouseY()), assetsLoader->cursor);

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		switch (game_state)
		{
		case GameStates::MENUE:
			if (!MenueLoop(deltaTime))
				return false;
			break;
		case GameStates::SETTINGS:
			SettingsLoop(deltaTime);
			break;
		case GameStates::LOADING_GAME:
			LoadingGameLoop(deltaTime);
			break;
		case GameStates::GAME:
			if (!GameLoop(deltaTime))
				return false;
			break;
		default:
			return false;
		}

		return true;
	}
};

int main()
{
	ShowCursor(false);
	Game game;
	if (game.Construct(416, 320, 1, 1, true, false)) // 640, 480 or  416, 320
		game.Start();

	return 0;
}