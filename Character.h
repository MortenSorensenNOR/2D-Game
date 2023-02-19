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
	olc::Decal* decal;
	olc::vi2d decalSize;
	olc::vi2d decalOffset = { 0, 0 };
	bool flipped;

	CharacterAnimator* characterAnimator;

	// Rendering
	RenderComponent* renderComponent;

	// Sound
	ISoundEngine* soundEngine;
	ISound* walkSound;

public:
	Character(olc::vf2d _Pos, olc::Decal* _CharacterDecal, olc::vi2d _CharacterDecalSize, vector<int>& character_frame_numbers, int directions, bool _Friendly)
	{
		// Movement
		pos = _Pos;
		
		// Traits
		friendly = _Friendly;

		// Sprite
		decal = _CharacterDecal;
		decalSize = _CharacterDecalSize;

		characterAnimator = new CharacterAnimator(character_frame_numbers, directions);

		// Rendering
		renderComponent = new RenderComponent(&pos, &decalSize, &decalOffset, decal, &flipped, 2);
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
		characterAnimator->GetOffset(decalOffset, flipped);
		decalOffset *= decalSize;

		dir = { 0.0f, 0.0f };
	}

	void Render(Camera* camera)
	{
		camera->RenderGameComponent(renderComponent);
	}
};

#endif // !CHARACTER_H