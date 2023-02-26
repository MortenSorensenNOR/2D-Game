#ifndef CHARACTER_H
#define CHARACTER_H

class Character : public GameElement
{
public:
	// Movement
	float walkSpeed = 70.0f;
	olc::vf2d dir = { 0.0f, 0.0f };

	// Animation
	CharacterAnimator* characterAnimator;

	// Sound
	irrklang::ik_f32 movementSound_time;
	irrklang::ik_f32 movementSound_length;
	irrklang::ISoundSource* movementSound;

public:
	Character(olc::vf2d _Pos, olc::vi2d _Size, olc::vi2d _DecalSize, olc::Decal* _Decal, olc::vi2d _DecalOffset, int _Shadow_offset, int _Id, vector<int>& Character_frame_numbers, irrklang::ISoundSource* MovementSound)
		: GameElement(_Pos, _Size, _DecalSize, _Decal, _DecalOffset, _Shadow_offset, _Id)
	{
		characterAnimator = new CharacterAnimator(Character_frame_numbers, 4);

		movementSound = MovementSound;
		movementSound_time = 0.0f;
		movementSound_length = movementSound->getPlayLength() / 1000.0f;
	}

	void MovementSound(float dt, irrklang::ISoundEngine* soundEngine)
	{
		if (movementSound_time < movementSound_length && movementSound_time != 0.0f)
			movementSound_time += dt;
		else
			movementSound_time = 0.0f;

		// Handle movement sound if dir != {0, 0}
		if (abs(dir.x) || abs(dir.y))
		{
			// Play movementsound if is not allready playing
			if (movementSound_time == 0.0f)
			{
				soundEngine->play2D(movementSound);
				movementSound_time += dt;
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