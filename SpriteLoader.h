#ifndef ASSETS_LOADER_H
#define ASSETS_LOADER_H

#include <vector>

#define OLC_PGE_APPLICATIONS
#include "olcPixelGameEngine.h"

class SpriteLoader
{
public:
	// Game icons
	olc::Sprite* cursor;
	olc::Sprite* lightTest;
	olc::Decal* decal_lightTest;

	// Floor sprites
	olc::Sprite* sprite_floor_wood;
	olc::Sprite* sprite_floor_grass_01;
	olc::Decal* decal_floor_grass_01;

	// Wall sprites
	olc::Sprite* sprite_wall_plains;
	olc::Decal* decal_wall_plains;

	// Game item sprites
	olc::Sprite* sprite_gameItem_chest;
	olc::Decal* decal_gameItem_chest;
	olc::Sprite* sprite_gameItem_objects;
	olc::Decal* decal_gameItem_objects;

	// Character sprites
	olc::Sprite* sprite_character_player;
	olc::Decal* decal_character_player;
	olc::Sprite* sprite_character_player_dust;

	SpriteLoader()
	{
		// Game icons
		//cursor = new olc::Sprite("./gfx/cursor_75.png");
		//lightTest = new olc::Sprite("./gfx/Light_test_bright.png");
		//decal_lightTest = new olc::Decal(lightTest);

		// Floor sprites
		sprite_floor_wood = new olc::Sprite("./gfx/Sprite_pack/tilesets/floors/wooden.png");
		sprite_floor_grass_01 = new olc::Sprite("./gfx/Sprite_pack/tilesets/decor_16x16.png");
		decal_floor_grass_01 = new olc::Decal(sprite_floor_grass_01);

		// Wall sprites
		sprite_wall_plains = new olc::Sprite("./gfx/Sprite_pack/tilesets/plains.png");
		decal_wall_plains = new olc::Decal(sprite_wall_plains);

		// Game item sprites
		sprite_gameItem_chest = new olc::Sprite("./gfx/Sprite_pack/objects/chest_01.png");
		decal_gameItem_chest = new olc::Decal(sprite_gameItem_chest);
		sprite_gameItem_objects = new olc::Sprite("./gfx/Sprite_pack/objects/objects.png");
		decal_gameItem_objects = new olc::Decal(sprite_gameItem_objects);

		// Character sprites
		sprite_character_player = new olc::Sprite("./gfx/Sprite_pack/characters/player_fixed.png");
		decal_character_player = new olc::Decal(sprite_character_player);
		sprite_character_player_dust = new olc::Sprite("./gfx/Sprite_pack/particles/dust_particles_01.png");
	}

	~SpriteLoader()
	{
		// Game icons
		free(cursor);

		// Floor sprites
		free(sprite_floor_wood);
		free(sprite_floor_grass_01);
		free(decal_floor_grass_01);

		// Wall sprites
		free(sprite_wall_plains);
		free(decal_wall_plains);

		// Game item sprites
		free(sprite_gameItem_chest);
		free(decal_gameItem_chest);
		free(sprite_gameItem_objects);
		free(decal_gameItem_objects);

		// Character sprites
		free(sprite_character_player);
		free(decal_character_player);
		free(sprite_character_player_dust);
	}
};

// Character sprites
std::vector<int> character_player_frame_numbers = { 6, 6, 4 };

#endif // !ASSETS_LOADER_H