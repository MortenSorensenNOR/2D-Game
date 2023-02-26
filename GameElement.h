#ifndef GAME_ELEMENT_H
#define GAME_ELEMENT_H

class GameElement
{
public:
	int id;

	// Position
	olc::vf2d pos;
	olc::vi2d size;

	// Rendering
	olc::Decal* decal;
	olc::vi2d decalSize;
	olc::vi2d decalOffset;
	bool flipped = false;

	RenderComponent* renderComponent;

public:
	GameElement(olc::vf2d _Pos, olc::vi2d _Size, olc::vi2d _DecalSize, olc::Decal* _Decal, olc::vi2d _DecalOffset, int _Shadow_offset, int _Id)
	{
		id = _Id;
		pos = _Pos;
		size = _Size;

		// Rendering
		decal = _Decal;
		decalSize = _DecalSize;
		decalOffset = _DecalOffset;

		renderComponent = new RenderComponent(&pos, &decalSize, &decalOffset, decal, &flipped, _Shadow_offset);
	}

	// Virual functions -> to be expanded on by derived classes
	virtual void HandleSound(float dt, irrklang::ISoundEngine* soundEngine) {}
	virtual void Update(float dt, olc::PixelGameEngine* game) {}

	// Universal render function
	void Render(Camera* camera) const
	{
		camera->RenderGameComponent(renderComponent);
	}
};

#endif // !GAME_ELEMENT_H