#ifndef CHARACTER_H
#define CHARACTER_H

class Character : public GameElement
{
public:
	// Movement
	float walkSpeed = 70.0f;
	olc::vf2d dir = { 0.0f, 0.0f };

	// Animation
	float time_between_animation_frames;
	CharacterAnimator* characterAnimator;

	// Sound
	bool movementSound_play_next_animation_tick = false;
	irrklang::ik_f32 movementSound_time;
	irrklang::ik_f32 movementSound_time_between;
	irrklang::ISoundSource* movementSound;

public:
	Character(olc::vf2d _Pos, olc::vi2d _Size, olc::vi2d _DecalSize, olc::Decal* _Decal, olc::vi2d _DecalOffset, int _Shadow_offset, int _Id, vector<int>& Character_frame_numbers, float Time_between_frames, irrklang::ISoundSource* MovementSound)
		: GameElement(_Pos, _Size, _DecalSize, _Decal, _DecalOffset, _Shadow_offset, _Id)
	{
		time_between_animation_frames = Time_between_frames;
		characterAnimator = new CharacterAnimator(Character_frame_numbers, 4, time_between_animation_frames);

		movementSound = MovementSound;
		movementSound_time = 0.0f;
		movementSound_time_between = irrklang::ik_f32(3 * time_between_animation_frames);
	}

	void MovementSound(float dt, irrklang::ISoundEngine* soundEngine)
	{
		if (movementSound_time < movementSound_time_between && movementSound_time != 0.0f)
			movementSound_time += dt;
		else
			movementSound_time = 0.0f;

		// Handle movement sound if dir != {0, 0}
		if (abs(dir.x) || abs(dir.y))
		{
			// Play movementsound if is not allready playing
			if (movementSound_time == 0.0f)
			{
				movementSound_time += dt;
				movementSound_play_next_animation_tick = true;
			}
			if (movementSound_time >= time_between_animation_frames && movementSound_play_next_animation_tick)
			{
				soundEngine->play2D(movementSound);
				movementSound_play_next_animation_tick = false;
			}
		}
	}

	void HandleAnimation(float dt)
	{
		// Handle character animation
		characterAnimator->Update(dt, dir);
		characterAnimator->GetOffset(decalOffset, flipped);
		decalOffset *= decalSize;
	}

	void HandleMovement(float dt)
	{
		// Handle character movement
		pos.x = pos.x + walkSpeed * dir.x * dt;
		pos.y = pos.y + walkSpeed * dir.y * dt;
	}
};

#endif // !CHARACTER_H