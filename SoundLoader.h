#ifndef SOUND_LOADER_H
#define SOUND_LOADER_H

class SoundLoader
{
public:
    ISoundSource* ui_click;
    ISound* background_music;
    ISoundSource* walk01;
    ISoundSource* walk02;

    SoundLoader(ISoundEngine* soundEngine)
    {
        // UI Sounds
        ui_click = soundEngine->addSoundSourceFromFile("./sfx/ui_click.mp3", ESM_AUTO_DETECT, true);
        if (ui_click)
        {
            ui_click->setDefaultVolume(0.69f);
        }

        // Music
        background_music = soundEngine->play2D("./sfx/intro_song.wav", true, false, true);
        if (background_music)
        {
            background_music->setVolume(0.42f);
        }

        // Walk sounds
        walk01 = soundEngine->addSoundSourceFromFile("./sfx/walk.wav", ESM_AUTO_DETECT, true);
        walk02 = soundEngine->addSoundSourceFromFile("./sfx/walk.wav", ESM_AUTO_DETECT, true);
    }

    void SetUIVolume(float volume)
    {
        if (ui_click)
        {
            ui_click->setDefaultVolume(volume);
        }
    }

    void SetMusicVolume(float volume)
    {
        if (background_music)
        {
            background_music->setVolume(volume);
        }
    }

    void SetSFXVolume(float volume)
    {
        if (walk01)
            walk01->setDefaultVolume(volume);
        if (walk02)
            walk02->setDefaultVolume(volume);
    }
};

#endif // !SOUND_LOADER_H