#include <map>
#include <queue>
#include <vector>
#include <string>
#include <time.h>
#include <chrono>
#include <stdlib.h>
using namespace std;

#include <irrKlang.h>
using namespace irrklang;

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Game headerfiles
#include "Camera.h"
#include "SpriteLoader.h"
#include "SoundLoader.h"
#include "Animator.h"
#include "GameElement.h"
#include "Level.h"
#include "Character.h"
#include "Player.h"
#include "Menu.h"

// Different game screens
enum GameScreens
{
    MENU = 0,
    SETTINGS = 1,
    AUDIO = 2,
    LOADING_GAME = 3,
    GAME = 4,
    QUIT = 5
};

struct LightParticle
{
    // Animation
    float animationTime = 0.0f;
    float positionTime = 0.0f;

    // Rendering
    olc::vf2d pos;
    olc::vi2d size;
    olc::Decal* decal;
    olc::vi2d offset;
    bool flipped;
    RenderComponent* renderComponent;

    LightParticle(olc::vf2d Pos, olc::vi2d Size, olc::Decal* Decal, olc::vi2d Offset, bool Flipped)
    {
        pos = Pos;
        size = Size;
        decal = Decal;
        offset = Offset;
        flipped = Flipped;
        renderComponent = renderComponent = new RenderComponent(&pos, &size, &offset, decal, &flipped, 0);
    }

    void Update(float dt)
    {
        animationTime += dt;
        positionTime += dt;
        pos.x += 7.5f * dt;
        pos.y = 144.0f + 32.0f * sinf(0.69 * positionTime);
        if (animationTime > 1.0f)
        {
            animationTime = 0.0f;
            offset.x = (offset.x + 32) % 64;
        }
    }

    void Render(Camera* camera)
    {
        camera->RenderGameComponent(renderComponent, false, true);
    }
};

class Game : public olc::PixelGameEngine
{
public:
    Game()
    {
        sAppName = "2D Game";
    }

public:
    //========================= MENUS =========================//
    //                                                         //
    //                                                         //
    //                                                         //
    //                                                         //
    //                         Play                            //
    //                         Settings                        //
    //                         Quit                            //
    //                                                         //
    //                                                         //
    //                                                         //
    //                                                         //
    //========================= MENUS =========================//
    int game_screen = GameScreens::MENU;

    // Main Menu
    Menu* mainMenu;
    vector<int> main_menu_redirect = { 3, 1, 5 };
    vector<string> main_menu_options = { "Play", "Settings", "Quit" };

    // Settings Menu
    Menu* settingsMenu;
    vector<int> settings_menu_redirect = { 1, 2, 0 };
    vector<string> settings_menu_options = { "Video", "Audio", "Back" };

    VolumeMenu* audioMenue;

    // Game Load Menu
    GameLoadMenu* gameLoadMenu;
    vector<int> game_load_menu_redirect = { 3, 1, 5 };
    vector<string> game_load_menu_options = { "Play", "Settings", "Quit" };

    //========================= GAME ==========================//
    //                                                         //
    //                                                         //
    //           ██████╗  █████╗ ███╗   ███╗███████╗           //
    //          ██╔════╝ ██╔══██╗████╗ ████║██╔════╝           //
    //          ██║  ███╗███████║██╔████╔██║█████╗             //
    //          ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝             //
    //          ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗           //
    //           ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝           //
    //                                                         //
    //                                                         //
    //                                                         //
    //========================= GAME ==========================//
    // Set default tilesize
    olc::vi2d defaultTileSize = { 16, 16 };

    // Set number of verticle and horizontal tiles
    int horizontalTileNum, verticleTileNum;

    // Assets Loader
    SpriteLoader* spriteLoader;

    // Camera
    Camera* camera;

    // Player
    Player* player;

    // Level
    Level* level;

    //========================= SOUND =========================//
    //                                                         //
    //                      /|||        ||\                    //
    //                     /||||    |\   \||\                  //
    //                 ||||||||| |\  ||\   \||                 //
    //                 ||||||||| \|\  \||   |||                //
    //                 |||||||||  ||   ||   |||                //
    //                 ||||||||| /|/  /||   |||                //
    //                 ||||||||| |/  ||/   /||                 //
    //                     \||||    |/   /||/                  //
    //                      \|||        ||/                    //
    //                                                         //
    //========================= SOUND =========================//
    // Sound Engine
    ISoundEngine* soundEngine = createIrrKlangDevice();
    SoundLoader* soundLoader;

    // Volumes
    ik_f32 sound_ui_volume = 0.5f;
    ik_f32 sound_music_volume = 0.5f;

public:
    bool OnUserCreate() override
    {
        // ======= SOUND =======
        if (!soundEngine) return false;
        soundLoader = new SoundLoader(soundEngine);

        // ========================= MENUS =========================
        mainMenu = new Menu(&game_screen, main_menu_options, main_menu_redirect, soundLoader->ui_click);
        settingsMenu = new Menu(&game_screen, settings_menu_options, settings_menu_redirect, soundLoader->ui_click);
        gameLoadMenu = new GameLoadMenu(&game_screen, game_load_menu_options, game_load_menu_redirect, soundLoader->ui_click, 1.5f, GameScreens::GAME);
        audioMenue = new VolumeMenu(&game_screen, GameScreens::MENU, ScreenHeight(), soundLoader);

        mainMenu->SetScreenSize(ScreenWidth(), ScreenHeight());
        settingsMenu->SetScreenSize(ScreenWidth(), ScreenHeight());
        gameLoadMenu->SetScreenSize(ScreenWidth(), ScreenHeight());

        // ========================= GAME =========================
        SetDecalMode(olc::DecalMode::MULTIPLICATIVE);
        horizontalTileNum = ScreenWidth() / defaultTileSize.x;
        verticleTileNum = ScreenHeight() / defaultTileSize.y;

        spriteLoader = new SpriteLoader();
        camera = new Camera(olc::vf2d(0.0f, 0.0f));
        player = new Player(olc::vf2d(64, 64), olc::vi2d(15, 23), olc::vi2d(15, 23), spriteLoader->decal_character_player, olc::vi2d(0, 0), 4, 0, character_player_frame_numbers, 0.120f, soundLoader->walk01);
        level = new Level(horizontalTileNum, verticleTileNum, spriteLoader);

        return true;
    }

    bool GameLoop(float dt)
    {
        if (GetKey(olc::Key::ESCAPE).bPressed)
        {
            game_screen = GameScreens::MENU;
            soundEngine->play2D(soundLoader->ui_click);
        }

        // ===== UPDATE =====
        player->Update(dt, this);

        // ===== SOUND =====
        player->HandleSound(dt, soundEngine);

        // ===== RENDER =====
        player->Render(camera);
        level->Render(camera);
        camera->Render(this);

        return true;
    }

    bool OnUserUpdate(float deltaTime) override
    {
        switch (game_screen)
        {
        case GameScreens::MENU:
            mainMenu->Update(this, soundEngine);
            mainMenu->Render(this);
            break;
        case GameScreens::SETTINGS:
            settingsMenu->Update(this, soundEngine);
            settingsMenu->Render(this);
            break;
        case GameScreens::AUDIO:
            audioMenue->Update(this, deltaTime);
            audioMenue->Render(this);
            break;
        case GameScreens::LOADING_GAME:
            gameLoadMenu->Update(deltaTime, camera);
            gameLoadMenu->Render(this);
            break;
        case GameScreens::GAME:
            if (!GameLoop(deltaTime))
                return false;
            break;
        case GameScreens::QUIT:
            return false;
            break;
        }

        return true;
    }

    bool OnUserDestroy() override
    {
        // Uninitilize audio
        soundEngine->drop();
        return true;
    }
};

int main()
{
    ShowCursor(false);
    Game game;
    if (game.Construct(416, 320, 1, 1, true, false)) // 640, 480 or  416, 320
        game.Start();

    return 0;
}