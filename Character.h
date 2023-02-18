#ifndef CHARACTER_H
#define CHARACTER_H

class Character
{
public:
	// Movement
	olc::vf2d pos;
	olc::vf2d dir;
	float walkSpeed = 75;

	// Character traits
	bool friendly = true;

	// Sprite Setup
	olc::Sprite* sprite;
	olc::vi2d spriteSize;
	olc::vi2d spriteOffset = { 0, 0 };
	bool flipped;

	std::pair<olc::vi2d, bool> spriteState;
	CharacterAnimator* characterAnimator;

	// Rendering
	RenderComponent* renderComponent;

	// Sound
	ISoundEngine* soundEngine;
	ISound* walkSound;

public:
	Character(olc::vf2d _Pos, olc::Sprite* _CharacterSprite, olc::vi2d _CharacterSpriteSize, vector<int>& character_frame_numbers, int directions, bool _Friendly)
	{
		// Movement
		pos = _Pos;
		
		// Traits
		friendly = _Friendly;

		// Sprite
		sprite = _CharacterSprite;
		spriteSize = _CharacterSpriteSize;

		characterAnimator = new CharacterAnimator(character_frame_numbers, directions);

		// Rendering
		renderComponent = new RenderComponent(&pos, &spriteSize, &spriteOffset, sprite, &flipped, 2);
	}

	void Update(float dt)
	{
		// Movement
		if (abs(dir.x) || abs(dir.y))
		{
			dir = dir.norm();
			pos.x = pos.x + walkSpeed * dir.x * dt;
			pos.y = pos.y + walkSpeed * dir.y* dt;
		}

		// Animation
		characterAnimator->Update(dt, dir);
		characterAnimator->GetOffset(spriteOffset, flipped);
		spriteOffset *= spriteSize;

		dir = { 0.0f, 0.0f };
	}

	void Render(Camera* camera)
	{
		camera->RenderGameComponent(renderComponent);
	}
};

#endif // !CHARACTER_H