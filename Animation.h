#ifndef ANIMATION_H
#define ANIMATION_H

enum PlayerAnimationStates
{
	IDLE_FRONT,
	IDLE_RIGHT,
	IDLE_LEFT,
	IDLE_BACK,
	WALK_FRONT,
	WALK_RIGHT,
	WALK_LEFT,
	WALK_BACK
};

enum EnemyAnimationStates;

class Animator
{
private:
	int animationState = 0;
	int animationFrame = 0;
	int maxAnimationFrame;

	float animationTime = 0.0f;
	float animationTriggerTime;

public:
	Animator(int iMaxAnimationFrame, float fAnimationTriggerTime)
	{
		maxAnimationFrame = iMaxAnimationFrame;
		animationTriggerTime = fAnimationTriggerTime;
	}

	int getAnimationState()
	{
		return animationState;
	}

	void updateWithDirection(float dt, olc::vf2d& dir)
	{
		if (dir.x == 0.0f && dir.y == 0.0f)
		{
			if (animationState == WALK_FRONT || animationState == IDLE_FRONT) {
				animationState = IDLE_FRONT;
			}
			else if (animationState == WALK_BACK || animationState == IDLE_BACK) {
				animationState = IDLE_BACK;
			}
			else if (animationState == WALK_LEFT || animationState == IDLE_LEFT) {
				animationState = IDLE_LEFT;
			}
			else if (animationState == WALK_RIGHT || animationState == IDLE_RIGHT) {
				animationState = IDLE_RIGHT;
			}
			else {
				animationState = IDLE_FRONT;
			}
		}
		else
		{
			if (abs(dir.x) < abs(dir.y))
			{
				if (dir.y < 0)
					animationState = WALK_FRONT;
				else
					animationState = WALK_BACK;
			}
			else if (abs(dir.x) > abs(dir.y))
			{
				if (dir.x < 0)
					animationState = WALK_RIGHT;
				else
					animationState = WALK_LEFT;
			}
			else
			{
				if (dir.y < 0)
					animationState = WALK_FRONT;
				else
					animationState = WALK_BACK;
			}
		}
		dir = { 0.0f, 0.0f };

		animationTime += dt;
		if (animationTime > animationTriggerTime)
		{
			animationFrame = (animationFrame + 1) % (maxAnimationFrame + 1);
			animationTime -= animationTriggerTime;
		}
	}

	void update(float dt)
	{
		animationTime += dt;
		if (animationTime > animationTriggerTime)
		{
			animationFrame = (animationFrame + 1) % (maxAnimationFrame + 1);
			animationTime -= animationTriggerTime;
		}
	}

	void render(olc::PixelGameEngine* game, olc::vf2d& pos, olc::Sprite* sprite, olc::vi2d& spriteSize)
	{
		if (animationState == PlayerAnimationStates::IDLE_FRONT) {
			game->DrawPartialSprite(pos, sprite, spriteSize * olc::vi2d(animationFrame, 0), spriteSize, 2);
		}
		else if (animationState == PlayerAnimationStates::IDLE_BACK) {
			game->DrawPartialSprite(pos, sprite, spriteSize * olc::vi2d(animationFrame, 2), spriteSize, 2);
		}
		else if (animationState == PlayerAnimationStates::IDLE_LEFT) {
			game->DrawPartialSprite(pos, sprite, spriteSize * olc::vi2d(animationFrame, 1), spriteSize, 2, olc::Sprite::HORIZ);
		}
		else if (animationState == PlayerAnimationStates::IDLE_RIGHT) {
			game->DrawPartialSprite(pos, sprite, spriteSize * olc::vi2d(animationFrame, 1), spriteSize, 2);
		}
		else if (animationState == PlayerAnimationStates::WALK_FRONT) {
			game->DrawPartialSprite(pos, sprite, spriteSize * olc::vi2d(animationFrame, 3), spriteSize, 2);
		}
		else if (animationState == PlayerAnimationStates::WALK_BACK) {
			game->DrawPartialSprite(pos, sprite, spriteSize * olc::vi2d(animationFrame, 5), spriteSize, 2);
		}
		else if (animationState == PlayerAnimationStates::WALK_LEFT) {
			game->DrawPartialSprite(pos, sprite, spriteSize * olc::vi2d(animationFrame, 4), spriteSize, 2, olc::Sprite::HORIZ);
		}
		else if (animationState == PlayerAnimationStates::WALK_RIGHT) {
			game->DrawPartialSprite(pos, sprite, spriteSize * olc::vi2d(animationFrame, 4), spriteSize, 2);
		}
		else {
			game->DrawPartialSprite(pos, sprite, spriteSize * olc::vi2d(animationFrame, 0), spriteSize, 2);
		}
	}
};

#endif // !ANIMATION_H
