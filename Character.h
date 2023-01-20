#ifndef CHARACTER_H
#define CHARACTER_H

#include "State.h"

enum WalkingDirections
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Character
{
public:
	olc::vf2d pos;
	bool walkingDir[4];

	bool friendly = true;
	KeyBoardState* keyboardState;

	// Animation
	int animationFrame = 0;
	int maxAnimationFrame = 5;
	float animationTime = 0;
	float nextAnimationTriggerTime = 1.0f / 6.0f;

	// Sprite Setup
	olc::Sprite* sprite;
	olc::vi2d spriteSize = { 48, 48 };

	Character(olc::vf2d startPos, olc::Sprite* characterSprite, bool isFriendly, KeyBoardState* gameKeyBoardState)
	{
		pos = startPos;
		sprite = characterSprite;
		friendly = isFriendly;
		keyboardState = gameKeyBoardState;
		for (int i = 0; i < 4; i++)
		{
			walkingDir[i] = false;
		}
	}

	~Character()
	{
		free(&pos);
		free(&friendly);
	}

	void update(float dt)
	{
		// Checks KeyBoardState for pressed keys
		// TODO: Make more efficient, stupid implementation
		if (keyboardState->keyStates[olc::Key::W]) {
			pos.y -= 1;
			walkingDir[UP] = true;
		}
		else {
			walkingDir[UP] = false;
		}
		if (keyboardState->keyStates[olc::Key::A]) {
			pos.x -= 1;
			walkingDir[LEFT] = true;
		}
		else {
			walkingDir[LEFT] = false;
		}
		if (keyboardState->keyStates[olc::Key::S]) {
			pos.y += 1;
			walkingDir[DOWN] = true;
		}
		else {
			walkingDir[DOWN] = false;
		}
		if (keyboardState->keyStates[olc::Key::D]) {
			pos.x += 1;
			walkingDir[RIGHT] = true;
		}
		else {
			walkingDir[RIGHT] = false;
		}

		animationTime += dt;
		if (animationTime > nextAnimationTriggerTime)
		{
			animationFrame = (animationFrame + 1) % (maxAnimationFrame + 1);
			animationTime -= nextAnimationTriggerTime;
		}
	}

	void render(olc::TransformedView& tv)
	{
		//tv.DrawSprite(pos, sprite);
		if (walkingDir[RIGHT] && !walkingDir[LEFT]) {
			tv.DrawPartialSprite(pos, sprite, spriteSize * olc::vi2d(animationFrame, 4), spriteSize);
		}
		else if (walkingDir[LEFT] && !walkingDir[RIGHT])
		{
			tv.DrawPartialSprite(pos, sprite, spriteSize * olc::vi2d(animationFrame, 4), spriteSize, olc::vf2d(1, 1), olc::Sprite::Flip::HORIZ);
		}
		else if (walkingDir[UP] && !walkingDir[DOWN]) {
			tv.DrawPartialSprite(pos, sprite, spriteSize * olc::vi2d(animationFrame, 5), spriteSize);
		}
		else if (walkingDir[DOWN] && !walkingDir[UP])
		{
			tv.DrawPartialSprite(pos, sprite, spriteSize * olc::vi2d(animationFrame, 3), spriteSize);
		}
		else 
		{
			tv.DrawPartialSprite(pos, sprite, spriteSize * olc::vi2d(animationFrame, 0), spriteSize);
		}
	}
};

#endif // !CHARACTER_H