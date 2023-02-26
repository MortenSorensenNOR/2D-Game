#ifndef CAMERA_H
#define CAMERA_H

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

struct CameraFadeHandler
{
	bool bFadeIn = true;
	float maxFadeInTime = 1.0f;
	float animationTimer = 1.0f;

	float dt;
	chrono::time_point<chrono::high_resolution_clock> lastFrame;
	chrono::time_point<chrono::high_resolution_clock> thisFrame;

	void FadeIn(float FadeTime)
	{
		if (!Complete())
			return;
		bFadeIn = true;
		maxFadeInTime = FadeTime;
		animationTimer = 0.0f;
		lastFrame = chrono::high_resolution_clock::now();
	}

	void FadeOut(float FadeTime)
	{
		if (!Complete())
			return;
		bFadeIn = false;
		maxFadeInTime = FadeTime;
		animationTimer = FadeTime;
		lastFrame = chrono::high_resolution_clock::now();
	}

	bool Complete()
	{
		if (bFadeIn)
		{
			if (animationTimer >= maxFadeInTime)
				return true;
			return false;
		}
		if (animationTimer <= 0.0f)
			return true;
		return false;
	}

	void Update()
	{
		thisFrame = chrono::high_resolution_clock::now();
		dt = float(chrono::duration_cast<chrono::microseconds>(thisFrame - lastFrame).count()) / 1000000.0f;
		lastFrame = thisFrame;

		if (!Complete())
		{
			if (bFadeIn)
				animationTimer = min(maxFadeInTime, animationTimer + dt);
			else
				animationTimer = max(0.0f, animationTimer - dt);
		}

	}

	void Render(olc::PixelGameEngine* game)
	{
		game->FillRectDecal(olc::vi2d(0, 0), olc::vi2d(game->ScreenWidth(), game->ScreenHeight()), olc::PixelF(0.1f, 0.1f, 0.1f, 1.0f - animationTimer / maxFadeInTime));
	}
};

class Camera
{
public:
	// Camera world position
	olc::vi2d cameraPosition;

	// Rendering Light
	int currentLightColor = 0;
	vector<olc::Pixel> lightColors = { olc::Pixel(255, 255, 255), olc::Pixel(80,104,134) };

	// Fade handler
	CameraFadeHandler fadeHandler;

	// Rendering queues
	queue<RenderComponent*> renderQueueBelow;
	queue<RenderComponent*> renderQueueLight;
	priority_queue<RenderComponent*, vector<RenderComponent*>, RenderComponent> renderQueue;


public:
	// Constructor
	Camera(olc::vf2d _CameraPosition)
	{
		cameraPosition = _CameraPosition;
	};

	// Adds game component to given rendering queue
	void RenderGameComponent(RenderComponent* component, bool renderBelow = false, bool renderLight = false)
	{
		if (renderBelow)
			renderQueueBelow.push(component);
		else if (renderLight)
			renderQueueLight.push(component);
		else
			renderQueue.push(component);
	}

	// Renders all game components: first all floor sprites, then all the rest in lower y-pos order
	void Render(olc::PixelGameEngine* game)
	{
		if (game->GetKey(olc::Key::L).bPressed)
		{
			if (fadeHandler.bFadeIn)
				fadeHandler.FadeOut(0.5f);
		}
		if (!fadeHandler.bFadeIn && fadeHandler.Complete())
		{
			currentLightColor = (currentLightColor + 1) % lightColors.size();
			fadeHandler.FadeIn(0.5f);
		}

		// Render below-components
		while (!renderQueueBelow.empty())
		{
			if (*(renderQueueBelow.front()->flipped))
				game->DrawPartialDecal(*renderQueueBelow.front()->pos + cameraPosition + olc::vf2d(renderQueue.top()->size->x, 0.0f), renderQueueBelow.front()->decal, *renderQueueBelow.front()->offset, *renderQueueBelow.front()->size, olc::vf2d(-1, 1), lightColors[currentLightColor]);
			else
				game->DrawPartialDecal(*renderQueueBelow.front()->pos + cameraPosition, renderQueueBelow.front()->decal, *renderQueueBelow.front()->offset, *renderQueueBelow.front()->size, olc::vf2d(1, 1), lightColors[currentLightColor]);
			renderQueueBelow.pop();
		}

		// Render the rest of the components
		while (!renderQueue.empty())
		{
			if (*(renderQueue.top()->flipped))
				game->DrawPartialDecal(*renderQueue.top()->pos + cameraPosition + olc::vf2d(renderQueue.top()->size->x, 0.0f), renderQueue.top()->decal, *renderQueue.top()->offset, *renderQueue.top()->size, olc::vf2d(-1, 1), lightColors[currentLightColor]);
			else
				game->DrawPartialDecal(*renderQueue.top()->pos + cameraPosition, renderQueue.top()->decal, *renderQueue.top()->offset, *renderQueue.top()->size, olc::vf2d(1, 1), lightColors[currentLightColor]);
			renderQueue.pop();
		}

		// Render Light components
		while (!renderQueueLight.empty())
		{
			game->DrawPartialDecal(*renderQueueLight.front()->pos + cameraPosition, renderQueueLight.front()->decal, *renderQueueLight.front()->offset, *renderQueueLight.front()->size);
			renderQueueLight.pop();
		}

		fadeHandler.Update();
		fadeHandler.Render(game);

		// FPS counter
		game->DrawStringDecal(olc::vf2d(0, 0), to_string(game->GetFPS()), olc::WHITE);
	}
};

#endif // !CAMERA_H