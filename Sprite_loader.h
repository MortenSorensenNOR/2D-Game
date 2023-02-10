#ifndef SPRITE_LOADER_H
#define SPRITE_LOADER_H

class SpriteLoader
{
public:
	// Floor sprites
	olc::Sprite* floor_wood;
	olc::Sprite* floor_grass_01;

	// Game item sprites
	olc::Sprite* gameItem_chest;

	// Character sprites
	olc::Sprite* character_player;
	olc::Sprite* character_player_dust;

	SpriteLoader()
	{
		// Floor sprites
		floor_wood = new olc::Sprite("./gfx/Sprite_pack/tilesets/floors/wooden.png");
		floor_grass_01 = new olc::Sprite("./gfx/Sprite_pack/tilesets/decor_16x16.png");

		// Game item sprites
		gameItem_chest = new olc::Sprite("./gfx/Sprite_pack/objects/chest_01.png");

		// Character sprites
		character_player = new olc::Sprite("./gfx/Sprite_pack/characters/player_fixed.png");
		character_player_dust = new olc::Sprite("./gfx/Sprite_pack/particles/dust_particles_01.png");
	}

	~SpriteLoader()
	{
		// Floor sprites
		free(floor_wood);
		free(floor_grass_01);
		
		// Game item sprites
		free(gameItem_chest);

		// Character sprites
		free(character_player);
		free(character_player_dust);
	}
};

// Character sprites
vector<int> character_player_frame_numbers = { 6, 6, 4 };

#endif // !SPRITE_LOADER_H