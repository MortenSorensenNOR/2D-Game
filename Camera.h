#ifndef CAMERA_H
#define CAMERA_H

#include <chrono>

struct RenderComponent
{
	olc::vf2d* pos;
	olc::vi2d* size;
	olc::vi2d* offset;
	olc::Decal* decal;
	int shadow_offset;
	bool* flipped;

	// Default constructor
	RenderComponent() 
	{
		pos = nullptr;
		size = nullptr;
		offset = nullptr;
		decal = nullptr;
		flipped = nullptr;
		shadow_offset = 0;
	}

	// Constructor
	RenderComponent(olc::vf2d* _Pos, olc::vi2d* _Size, olc::vi2d* _Offset, olc::Decal* _Sprite, bool* _Flipped, int _ShadowOffset)
	{
		pos = _Pos;
		size = _Size;
		offset = _Offset;
		decal = _Sprite;
		flipped = _Flipped;
		shadow_offset = _ShadowOffset;
	}

	// Sorting function for rendering queue
	bool operator()(RenderComponent* a, RenderComponent* b)
	{
		return (a->pos->y + a->size->y - a->shadow_offset) > (b->pos->y + b->size->y - b->shadow_offset);
	}
};

class Camera
{
private:
	// Fade in settings
	bool fadeIn;
	bool hasFadedIn = false;
	float fade_in_time = 0.0f;
	float max_fade_in_time = 0.5f;
	chrono::time_point<chrono::high_resolution_clock> lastFrame;

	// Camera internal settigs
	olc::vi2d cameraIntrinsics;
	olc::vf2d* playerPosition;

	// Rendering queues
	queue<RenderComponent*> renderQueueBelow;
	priority_queue<RenderComponent*, vector<RenderComponent*>, RenderComponent> renderQueue;

	int currentLightColor = 0;
	vector<olc::Pixel> lightColors = { olc::Pixel(80,104,134), olc::Pixel(255, 255, 255) };

public:
	// Constructor
	Camera(olc::vf2d* _PlayerPosition, olc::vi2d _CameraIntrinsics, bool _FadeIn)
	{
		fadeIn = _FadeIn;
		playerPosition = _PlayerPosition;
		cameraIntrinsics = _CameraIntrinsics;
	};

	void Init(olc::PixelGameEngine* game)
	{
		game->SetDecalMode(olc::DecalMode::MULTIPLICATIVE);
		lastFrame = chrono::high_resolution_clock::now();
	}

	// Get the current camera offset for rendering sprite
	olc::vf2d GetCameraOffset()
	{
		return olc::vf2d(-1.0f * (*playerPosition).x + (float)cameraIntrinsics.x / 2.0f, -1.0f * (*playerPosition).y + (float)cameraIntrinsics.y / 2.0f);
	}

	// Adds game component to given rendering queue
	void RenderGameComponent(RenderComponent* component, bool renderBelow = false)
	{
		if (renderBelow)
			renderQueueBelow.push(component);
		else
			renderQueue.push(component);
	}

	// Renders all game components: first all floor sprites, 
	// then all the rest in lower y-pos order
	void Render(olc::PixelGameEngine* game, AssetsLoader* assetsLoader)
	{
		// Gets the the current camera offset
		olc::vf2d cameraOffset = GetCameraOffset();

		if (game->GetKey(olc::Key::L).bPressed)
			currentLightColor = (currentLightColor + 1) % lightColors.size();

		// Render below-components
		while (!renderQueueBelow.empty())
		{
			if (*(renderQueueBelow.front()->flipped))
				game->DrawPartialDecal(*renderQueueBelow.front()->pos + cameraOffset, renderQueueBelow.front()->decal, *renderQueueBelow.front()->offset, *renderQueueBelow.front()->size, olc::vf2d(1, 1), lightColors[currentLightColor]);
			else
				game->DrawPartialDecal(*renderQueueBelow.front()->pos + cameraOffset, renderQueueBelow.front()->decal, *renderQueueBelow.front()->offset, *renderQueueBelow.front()->size, olc::vf2d(1, 1), lightColors[currentLightColor]);
			renderQueueBelow.pop();
		}
		const olc::vf2d uvPointsFlipped[4] = {olc::vf2d(1.0f, 0.0f), olc::vf2d(0.0f, 0.0f), olc::vf2d(1.0f, 1.0f), olc::vf2d(0.0f, 1.0f)};
		// Render the rest of the components
		while (!renderQueue.empty())
		{
			if (*(renderQueue.top()->flipped))
				game->DrawPartialDecal(*renderQueue.top()->pos + cameraOffset, renderQueue.top()->decal, *renderQueue.top()->offset, *renderQueue.top()->size, olc::vf2d(1, 1), lightColors[currentLightColor]);
			else
				game->DrawPartialDecal(*renderQueue.top()->pos + cameraOffset, renderQueue.top()->decal, *renderQueue.top()->offset, *renderQueue.top()->size, olc::vf2d(1, 1), lightColors[currentLightColor]);
			renderQueue.pop();
		}

		if (fadeIn)
		{
			if (fade_in_time < max_fade_in_time)
			{
				game->FillRect(olc::vi2d(0, 0), olc::vi2d(game->ScreenWidth(), game->ScreenHeight()), olc::PixelF(0.1f, 0.1f, 0.1f, 1.0f - fade_in_time / max_fade_in_time));
				auto thisFrame = chrono::high_resolution_clock::now();
				fade_in_time += float(chrono::duration_cast<chrono::milliseconds>(thisFrame - lastFrame).count()) / 1000.0f;
				lastFrame = thisFrame;
			}
		}
	}

	/*
	* 
	* Note to self: Code amplifies small erros between the camera position and the player position
	* with the way it is currently implemented, and as such should be rewritten to midigate this
	* effect.
	*
	*/

};

#endif // !CAMERA_H
