#ifndef ANIMATOR_H
#define ANIMATOR_H

enum AnimationDirections {
	UP,
	DOWN,
	RIGHT,
	LEFT
};

class CharacterAnimator
{
private:
	int animationFrame = 0;
	int animationState = 0;
	int animationDirection = AnimationDirections::DOWN;
	float animationTime = 0.0f;
	float time_between_frames = 0.125f;
	vector<int>* animation_frame_numbers;

public:
	CharacterAnimator(vector<int>& Animation_frame_numbers, int Directions, float Time_Between_frames)
	{
		animation_frame_numbers = &Animation_frame_numbers;
		time_between_frames = Time_Between_frames;
	}

	void Update(float dt, olc::vf2d& dir)
	{
		animationTime += dt;
		if (abs(dir.x) && abs(dir.y))
			time_between_frames = 0.105f;
		else
			time_between_frames = 0.120f;
		if (animationTime > time_between_frames)
		{
			animationTime -= time_between_frames;
			animationFrame = (animationFrame + 1) % (*animation_frame_numbers)[animationState];
		}

		if (dir.x == 0 && dir.y == 0)
		{
			animationState = 0;
		}
		else
		{
			animationState = 1;
			if (dir.y == 0)
			{
				// Moving sideways
				if (dir.x > 0)
					animationDirection = AnimationDirections::RIGHT;
				else
					animationDirection = AnimationDirections::LEFT;
			}
			else
			{
				// Moving up or down
				if (dir.y < 0)
					animationDirection = AnimationDirections::UP;
				else
					animationDirection = AnimationDirections::DOWN;
			}
		}
	}

	void GetOffset(olc::vi2d& offset, bool& flipped)
	{
		if (animationDirection == AnimationDirections::LEFT)
		{
			offset.x = animationFrame;
			offset.y = animationState * 3 + 2;
			flipped = true;
		}
		else
		{
			offset.x = animationFrame;
			offset.y = animationState * 3 + animationDirection;
			flipped = false;
		}
	}
};

#endif // !ANIMATOR_H