#ifndef MENU_H
#define MENU_H

class Menu
{
protected:
	int* gameScreen;
	int menu_current_option = 0;
	std::vector<std::string> options;
	std::vector<int> buttonRedirections;

	int menu_font_size = 2;
	int menu_option_offset_top = 0;
	int menu_option_offset_left = 0;
	int menu_option_offset_between = 0;

	irrklang::ISoundSource* ui_click;

public:
	Menu(int* state, std::vector<std::string> Options, std::vector<int> ButtonRedirections, irrklang::ISoundSource* UI_click_sound)
	{
		// Buttons
		gameScreen = state;
		options = Options;
		buttonRedirections = ButtonRedirections;

		// Sound
		ui_click = UI_click_sound;
	}

	void SetScreenSize(int ScreenWidth, int ScreenHeight)
	{
		menu_option_offset_top = ScreenHeight / 2 - 35;
		menu_option_offset_left = ScreenWidth / 2 - 65;
		menu_option_offset_between = ScreenHeight / 12;
	}

	void Update(olc::PixelGameEngine* game, irrklang::ISoundEngine* soundEngine)
	{
		// ======= USER INPUT =======
		if (game->GetKey(olc::Key::UP).bPressed || game->GetKey(olc::Key::W).bPressed)
			menu_current_option = std::max(0, menu_current_option - 1);
		if (game->GetKey(olc::Key::DOWN).bPressed || game->GetKey(olc::Key::S).bPressed)
			menu_current_option = std::min(int(options.size() - 1), menu_current_option + 1);
		if (game->GetKey(olc::Key::ENTER).bPressed)
			soundEngine->play2D(ui_click);
		if (game->GetKey(olc::Key::ENTER).bReleased)
		{
			*gameScreen = buttonRedirections[menu_current_option];
		}
	}

	void Render(olc::PixelGameEngine* game)
	{
		// ======= RENDERING =======
		game->Clear(olc::PixelF(0.1, 0.1, 0.1, 1.0));
		for (int i = 0; i < options.size(); i++)
		{
			if (i == menu_current_option)
			{
				int shadow_offset = (game->GetKey(olc::Key::ENTER).bPressed || game->GetKey(olc::Key::ENTER).bHeld) ? 1 : 0;
				game->DrawString(olc::vi2d(menu_option_offset_left + 1, menu_option_offset_top + i * menu_option_offset_between + 1), options[i] + " <--", olc::GREY, menu_font_size);
				game->DrawString(olc::vi2d(menu_option_offset_left + shadow_offset, menu_option_offset_top + i * menu_option_offset_between + shadow_offset), options[i] + " <--", olc::WHITE, menu_font_size);
			}
			else
			{
				game->DrawString(olc::vi2d(menu_option_offset_left, menu_option_offset_top + i * menu_option_offset_between), options[i], olc::WHITE, menu_font_size);
			}
		}
	}
};

class GameLoadMenu : protected Menu
{
private:
	int gameScreen_game;
	float maxFadeInTime;
	float fadeInTime = 0.0f;

public:
	GameLoadMenu(int* state, std::vector<std::string> Options, std::vector<int> ButtonRedirections, irrklang::ISoundSource* UI_click_sound, float FadeInTime, int GameState_game) : Menu(state, Options, ButtonRedirections, UI_click_sound)
	{
		gameScreen_game = GameState_game;
		maxFadeInTime = FadeInTime;
	}

	void SetScreenSize(int ScreenWidth, int ScreenHeight)
	{
		menu_option_offset_top = ScreenHeight / 2 - 35;
		menu_option_offset_left = ScreenWidth / 2 - 65;
		menu_option_offset_between = ScreenHeight / 12;
	}

	void Update(float dt, Camera* camera)
	{
		if (fadeInTime < maxFadeInTime)
			fadeInTime += dt;
		else
		{
			*gameScreen = gameScreen_game;
			camera->fadeHandler.FadeIn(0.5f);
		}
	}

	void Render(olc::PixelGameEngine* game)
	{
		// ======= RENDERING =======
		float alpha = 1.0f - fadeInTime / maxFadeInTime;
		game->Clear(olc::PixelF(0.1, 0.1, 0.1, 1.0));
		for (int i = 0; i < options.size(); i++)
		{
			if (i == menu_current_option)
			{
				int shadow_offset = (game->GetKey(olc::Key::ENTER).bPressed || game->GetKey(olc::Key::ENTER).bHeld) ? 1 : 0;
				game->DrawString(olc::vi2d(menu_option_offset_left + 1, menu_option_offset_top + i * menu_option_offset_between + 1), options[i] + " <--", olc::PixelF(0.5f, 0.5f, 0.5f, alpha), menu_font_size);
				game->DrawString(olc::vi2d(menu_option_offset_left + shadow_offset, menu_option_offset_top + i * menu_option_offset_between + shadow_offset), options[i] + " <--", olc::PixelF(1.0f, 1.0f, 1.0f, alpha), menu_font_size);
			}
			else
			{
				game->DrawString(olc::vi2d(menu_option_offset_left, menu_option_offset_top + i * menu_option_offset_between), options[i], olc::PixelF(1.0f, 1.0f, 1.0f, alpha), menu_font_size);
			}
		}
	}
};

class SettingsSlider
{
private:
	bool selected = false;

	string name;
	olc::vi2d pos;
	olc::vi2d barSize;
	olc::vi2d offset = { 1, 1 };
	olc::vi2d border = { 2, 2 };

	olc::vf2d ballPosition;

public:
	SettingsSlider(string OptionName, olc::vi2d _Pos, olc::vi2d _BarSize, float defaultValue)
	{
		name = OptionName;
		pos = _Pos;
		barSize = _BarSize;
		ballPosition = { float(pos.x - 1) + float(barSize.x) * defaultValue, float(pos.y + barSize.y / 2) };
	}

	void SetSelected(bool Selected)
	{
		selected = Selected;
	}

	float Update(float dt, olc::PixelGameEngine* game)
	{
		if (selected)
		{
			if (game->GetKey(olc::Key::RIGHT).bPressed || game->GetKey(olc::Key::RIGHT).bHeld || game->GetKey(olc::Key::D).bPressed || game->GetKey(olc::Key::D).bHeld)
				ballPosition.x = min(float(pos.x + barSize.x - 1), ballPosition.x + 75.0f * dt);
			if (game->GetKey(olc::Key::LEFT).bPressed || game->GetKey(olc::Key::LEFT).bHeld || game->GetKey(olc::Key::A).bPressed || game->GetKey(olc::Key::A).bHeld)
				ballPosition.x = max(float(pos.x - 1), ballPosition.x - 75.0f * dt);
		}

		return (ballPosition.x - pos.x + 1) / barSize.x;
	}

	void Render(olc::PixelGameEngine* game)
	{
		string percent = to_string((ballPosition.x - pos.x + 1) / barSize.x * 100);
		game->DrawString(olc::vi2d(pos.x, pos.y - 3 - 15) + offset, name + ":" + percent.substr(0, percent.find(".")) + "%", selected ? olc::WHITE : olc::Pixel(225, 225, 225), 1);
		game->DrawString(olc::vi2d(pos.x - 15 - 12, pos.y - 3) + offset, "<", olc::GREY, 2);
		if (selected && (game->GetKey(olc::Key::LEFT).bPressed || game->GetKey(olc::Key::LEFT).bHeld || game->GetKey(olc::Key::A).bPressed || game->GetKey(olc::Key::A).bHeld))
			game->DrawString(olc::vi2d(pos.x - 15 - 12, pos.y - 3) + offset, "<", olc::WHITE, 2);
		else
			game->DrawString(olc::vi2d(pos.x - 15 - 12, pos.y - 3), "<", selected ? olc::WHITE : olc::Pixel(235, 225, 225), 2);

		game->DrawString(olc::vi2d(pos.x + 15 + barSize.x, pos.y - 3) + offset, ">", olc::GREY, 2);
		if (selected && (game->GetKey(olc::Key::RIGHT).bPressed || game->GetKey(olc::Key::RIGHT).bHeld || game->GetKey(olc::Key::D).bPressed || game->GetKey(olc::Key::D).bHeld))
			game->DrawString(olc::vi2d(pos.x + 15 + barSize.x, pos.y - 3) + offset, ">", olc::WHITE, 2);
		else
			game->DrawString(olc::vi2d(pos.x + 15 + barSize.x, pos.y - 3), ">", selected ? olc::WHITE : olc::Pixel(225, 225, 225), 2);

		// Track
		game->FillRect(pos - border, barSize + 2 * border, olc::GREY);
		game->FillCircle(olc::vi2d(pos.x - border.x, pos.y + barSize.y / 2), barSize.y / 2 + border.y, olc::GREY);
		game->FillCircle(olc::vi2d(pos.x + barSize.x, pos.y + barSize.y / 2), barSize.y / 2 + border.y, olc::GREY);

		game->FillRect(pos - olc::vi2d(1, 0), barSize, selected ? olc::WHITE : olc::Pixel(225, 225, 225));
		game->FillCircle(olc::vi2d(pos.x - 1, pos.y + barSize.y / 2), barSize.y / 2, selected ? olc::WHITE : olc::Pixel(225, 225, 225));
		game->FillCircle(olc::vi2d(pos.x + barSize.x - 1, pos.y + barSize.y / 2), barSize.y / 2, selected ? olc::WHITE : olc::Pixel(225, 225, 225));

		// Ball
		game->FillCircle(ballPosition, barSize.y / 2, olc::DARK_GREY);
	}
};

class VolumeMenu
{
public:
	int* gameScreen;
	int gameScreen_menu;
	int currentSelected = 0;

	SettingsSlider* ui_volume_slider;
	SettingsSlider* music_volume_slider;
	SettingsSlider* sfx_volume_slider;

	SoundLoader* soundLoader;
	float UI_volume = 0.69f;
	float Music_volume = 0.42f;
	float SFX_volume = 1.0f;

	VolumeMenu(int* state, int _GameScreen_menu, int ScreenHeight, SoundLoader* _SoundLoader)
	{
		gameScreen = state;
		gameScreen_menu = _GameScreen_menu;

		int sliderHeight = 9;
		int sliderWidth = 200;
		ui_volume_slider = new SettingsSlider("UI", olc::vi2d(sliderWidth / 2, ScreenHeight / 2 - sliderHeight / 2 - 50), olc::vi2d(sliderWidth, sliderHeight), UI_volume);
		music_volume_slider = new SettingsSlider("MUSIC", olc::vi2d(sliderWidth / 2, ScreenHeight / 2 - sliderHeight / 2), olc::vi2d(sliderWidth, sliderHeight), Music_volume);
		sfx_volume_slider = new SettingsSlider("SFX", olc::vi2d(sliderWidth / 2, ScreenHeight / 2 - sliderHeight / 2 + 50), olc::vi2d(sliderWidth, sliderHeight), SFX_volume);
		ui_volume_slider->SetSelected(true);

		soundLoader = _SoundLoader;
	}

	void Update(olc::PixelGameEngine* game, float dt)
	{
		if (game->GetKey(olc::Key::ESCAPE).bPressed)
			*gameScreen = gameScreen_menu;
		if (game->GetKey(olc::Key::DOWN).bPressed || game->GetKey(olc::Key::S).bPressed)
		{
			switch (currentSelected)
			{
			case 0:
				ui_volume_slider->SetSelected(false);
				music_volume_slider->SetSelected(true);
				sfx_volume_slider->SetSelected(false);
				currentSelected = 1;
				break;
			case 1:
				ui_volume_slider->SetSelected(false);
				music_volume_slider->SetSelected(false);
				sfx_volume_slider->SetSelected(true);
				currentSelected = 2;
			default:
				break;
			}
		}
		if (game->GetKey(olc::Key::UP).bPressed || game->GetKey(olc::Key::W).bPressed)
		{
			switch (currentSelected)
			{
			case 1:
				ui_volume_slider->SetSelected(true);
				music_volume_slider->SetSelected(false);
				sfx_volume_slider->SetSelected(false);
				currentSelected = 0;
				break;
			case 2:
				ui_volume_slider->SetSelected(false);
				music_volume_slider->SetSelected(true);
				sfx_volume_slider->SetSelected(false);
				currentSelected = 1;
				break;
			default:
				break;
			}
		}
		float new_UI_volume = ui_volume_slider->Update(dt, game);
		float new_Music_volume = music_volume_slider->Update(dt, game);
		float new_SFX_volume = sfx_volume_slider->Update(dt, game);
		if (new_UI_volume != UI_volume)
		{
			UI_volume = new_UI_volume;
			soundLoader->SetUIVolume(UI_volume);
		}
		if (new_Music_volume != Music_volume)
		{
			Music_volume = new_Music_volume;
			soundLoader->SetMusicVolume(Music_volume);
		}
		if (new_SFX_volume != SFX_volume)
		{
			SFX_volume = new_SFX_volume;
			soundLoader->SetSFXVolume(SFX_volume);
		}
	}

	void Render(olc::PixelGameEngine* game)
	{
		game->Clear(olc::PixelF(0.1, 0.1, 0.1));
		ui_volume_slider->Render(game);
		music_volume_slider->Render(game);
		sfx_volume_slider->Render(game);
	}
};

#endif // !MENU_H