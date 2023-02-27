#ifndef PLAYER_H
#define PLAYER_H

class Player : public Character
{
protected:

public:
	Player(olc::vf2d _Pos, olc::vi2d _Size, olc::vi2d _DecalSize, olc::Decal* _Decal, olc::vi2d _DecalOffset, int _Shadow_offset, int _Id, vector<int>& Character_frame_numbers, float Time_between_frames, irrklang::ISoundSource* MovementSound)
		: Character(_Pos, _Size, _DecalSize, _Decal, _DecalOffset, _Shadow_offset, _Id, Character_frame_numbers, Time_between_frames, MovementSound) {}

	void HandleUserInput(olc::PixelGameEngine* game)
	{
		// Handle user input
		if (game->GetKey(olc::Key::W).bPressed || game->GetKey(olc::Key::W).bHeld || game->GetKey(olc::Key::UP).bPressed || game->GetKey(olc::Key::UP).bHeld)
		{
			// UP
			dir += olc::vf2d(0.0f, -1.0f);
		}
		if (game->GetKey(olc::Key::S).bPressed || game->GetKey(olc::Key::S).bHeld || game->GetKey(olc::Key::DOWN).bPressed || game->GetKey(olc::Key::DOWN).bHeld)
		{
			// DOWN
			dir += olc::vf2d(0.0f, 1.0f);
		}
		if (game->GetKey(olc::Key::A).bPressed || game->GetKey(olc::Key::A).bHeld || game->GetKey(olc::Key::LEFT).bPressed || game->GetKey(olc::Key::LEFT).bHeld)
		{
			// LEFT
			dir += olc::vf2d(-1.0f, 0.0f);
		}
		if (game->GetKey(olc::Key::D).bPressed || game->GetKey(olc::Key::D).bHeld || game->GetKey(olc::Key::RIGHT).bPressed || game->GetKey(olc::Key::RIGHT).bHeld)
		{
			// RIGHT
			dir += olc::vf2d(1.0f, 0.0f);
		}
		// Prevents devide-by-0-error
		if (abs(dir.x) || abs(dir.y))
			dir = dir.norm();
	}

	void HandleSound(float dt, irrklang::ISoundEngine* soundEngine) override
	{
		// Handle character sound
		MovementSound(dt, soundEngine);
	}

	void Update(float dt, olc::PixelGameEngine* game) override
	{
		dir.x = 0.0f; dir.y = 0.0f;
		HandleUserInput(game);
		HandleAnimation(dt);
		HandleMovement(dt);
	}
};

#endif // !PLAYER_H