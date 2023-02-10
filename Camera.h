#ifndef CAMERA_H
#define CAMERA_H

struct RenderComponent
{
	olc::vf2d* pos;
	olc::vi2d* size;
	olc::vi2d* offset;
	olc::Sprite* sprite;
	bool* flipped;

	// Default constructor
	RenderComponent() 
	{
		pos = nullptr;
		size = nullptr;
		offset = nullptr;
		sprite = nullptr;
		flipped = nullptr;
	}

	// Constructor
	RenderComponent(olc::vf2d* _Pos, olc::vi2d* _Size, olc::vi2d* _Offset, olc::Sprite* _Sprite, bool* _Flipped)
	{
		pos = _Pos;
		size = _Size;
		offset = _Offset;
		sprite = _Sprite;
		flipped = _Flipped;
	}

	// Sorting function for rendering queue
	bool operator()(RenderComponent* a, RenderComponent* b)
	{
		return (a->pos->y + a->size->y) > (b->pos->y + b->size->y);
	}
};

class Camera
{
private:
	// Camera internal settigs
	olc::vi2d cameraIntrinsics;
	olc::vf2d* playerPosition;

	// Rendering queues
	queue<RenderComponent*> renderQueueBelow;
	priority_queue<RenderComponent*, vector<RenderComponent*>, RenderComponent> renderQueue;

public:
	// Constructor
	Camera(olc::vf2d* _PlayerPosition, olc::vi2d _CameraIntrinsics)
	{
		playerPosition = _PlayerPosition;
		cameraIntrinsics = _CameraIntrinsics;
	};

	// Get the current camera offset for rendering sprite
	olc::vf2d getCameraOffset()
	{
		return olc::vf2d(-1.0f * (*playerPosition).x + (float)cameraIntrinsics.x / 2.0f, -1.0f * (*playerPosition).y + (float)cameraIntrinsics.y / 2.0f);
	}

	// Adds game component to given rendering queue
	void renderGameComponent(RenderComponent* component, bool renderBelow = false)
	{
		if (renderBelow)
			renderQueueBelow.push(component);
		else
			renderQueue.push(component);
	}

	// Renders all game components: first all floor sprites, 
	// then all the rest in lower y-pos order
	void render(olc::PixelGameEngine* game)
	{
		// Gets the the current camera offset
		olc::vf2d cameraOffset = getCameraOffset();

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
