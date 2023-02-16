#ifndef ASSETS_LOADER_H
#define ASSETS_LOADER_H

class AssetsLoader
{
public:
	// Game icons
	olc::Sprite* cursor;

	// Floor sprites
	olc::Sprite* floor_wood;
	olc::Sprite* floor_grass_01;

	// Wall sprites
	olc::Sprite* wall_plains;

	// Game item sprites
	olc::Sprite* gameItem_chest;
	olc::Sprite* gameItem_objects;

	// Character sprites
	olc::Sprite* character_player;
	olc::Sprite* character_player_dust;

	AssetsLoader()
	{
		// Game icons
		//cursor = new olc::Sprite("./gfx/cursor_75.png");

		// Floor sprites
		floor_wood = new olc::Sprite("./gfx/Sprite_pack/tilesets/floors/wooden.png");
		floor_grass_01 = new olc::Sprite("./gfx/Sprite_pack/tilesets/decor_16x16.png");

		// Wall sprites
		wall_plains = new olc::Sprite("./gfx/Sprite_pack/tilesets/plains.png");

		// Game item sprites
		gameItem_chest = new olc::Sprite("./gfx/Sprite_pack/objects/chest_01.png");
		gameItem_objects = new olc::Sprite("./gfx/Sprite_pack/objects/objects.png");

		// Character sprites
		character_player = new olc::Sprite("./gfx/Sprite_pack/characters/player_fixed.png");
		character_player_dust = new olc::Sprite("./gfx/Sprite_pack/particles/dust_particles_01.png");
	}

	~AssetsLoader()
	{
		// Game icons
		free(cursor);

		// Floor sprites
		free(floor_wood);
		free(floor_grass_01);

		// Wall sprites
		free(wall_plains);
		
		// Game item sprites
		free(gameItem_chest);
		free(gameItem_objects);

		// Character sprites
		free(character_player);
		free(character_player_dust);
	}
};

// Character sprites
vector<int> character_player_frame_numbers = { 6, 6, 4 };

#endif // !ASSETS_LOADER_H