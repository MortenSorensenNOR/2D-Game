#ifndef CAMERA_H
#define CAMERA_H

#include <chrono>

struct RenderComponent
{
	olc::vf2d* pos;
	olc::vi2d* size;
	olc::vi2d* offset;
	olc::Sprite* sprite;
	int shadow_offset;
	bool* flipped;

	// Default constructor
	RenderComponent() 
	{
		pos = nullptr;
		size = nullptr;
		offset = nullptr;
		sprite = nullptr;
		flipped = nullptr;
		shadow_offset = 0;
	}

	// Constructor
	RenderComponent(olc::vf2d* _Pos, olc::vi2d* _Size, olc::vi2d* _Offset, olc::Sprite* _Sprite, bool* _Flipped, int _ShadowOffset)
	{
		pos = _Pos;
		size = _Size;
		offset = _Offset;
		sprite = _Sprite;
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

public:
	// Constructor
	Camera(olc::vf2d* _PlayerPosition, olc::vi2d _CameraIntrinsics, bool _FadeIn)
	{
		fadeIn = _FadeIn;
		playerPosition = _PlayerPosition;
		cameraIntrinsics = _CameraIntrinsics;
	};

	void Init()
	{
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
	void Render(olc::PixelGameEngine* game)
	{
		// Gets the the current camera offset
		olc::vf2d cameraOffset = GetCameraOffset();

		// Render below-components
		while (!renderQueueBelow.empty())
		{
			if (*(renderQueueBelow.front()->flipped))
				game->DrawPartialSprite(*renderQueueBelow.front()->pos + cameraOffset, renderQueueBelow.front()->sprite, *renderQueueBelow.front()->offset, *renderQueueBelow.front()->size, 1, olc::Sprite::HORIZ);
			else
				game->DrawPartialSprite(*renderQueueBelow.front()->pos + cameraOffset, renderQueueBelow.front()->sprite, *renderQueueBelow.front()->offset, *renderQueueBelow.front()->size);
			renderQueueBelow.pop();
		}

		// Render the rest of the components
		while (!renderQueue.empty())
		{
			if (*(renderQueue.top()->flipped))
				game->DrawPartialSprite(*renderQueue.top()->pos + cameraOffset, renderQueue.top()->sprite, *renderQueue.top()->offset, *renderQueue.top()->size, 1, olc::Sprite::HORIZ);
			else
				game->DrawPartialSprite(*renderQueue.top()->pos + cameraOffset, renderQueue.top()->sprite, *renderQueue.top()->offset, *renderQueue.top()->size);
			renderQueue.pop();
		}

		if (fadeIn)
		{
			if (fade_in_time < max_fade_in_time)
			{
				game->FillRect(olc::vi2d(0, 0), olc::vi2d(game->ScreenWidth(), game->ScreenHeight()), olc::PixelF(0.0f, 0.0f, 0.0f, 1.0f - fade_in_time / max_fade_in_time));
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
