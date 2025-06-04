#pragma once
#include "raylib.h"
#include "assets.h"

enum GameState
{
    MAINMENU,
    PLAYMODE,
    ENDGAME
};

class GameManager
{
private:
    GameManager() {};
    GameState currentState;
    Music music;

public:
    GameManager(GameManager const &) = delete;
    void operator=(GameManager const &) = delete;
    static GameManager &GetInstance()
    {
        static GameManager instance;
        return instance;
    }

    bool OnPlayMode() { return currentState == PLAYMODE; }
    bool OnMainMenu() { return currentState == MAINMENU; }
    bool OnEndGame() { return currentState == ENDGAME; }
    void SetMusic(const char *musicSource)
    {
        music = LoadMusicStream(musicSource);
    }
    void MoveToMainMenu();
    Music GetMusic()
    {
        if (music.frameCount == 0)
        {
            // If music is not loaded, load it from assets
            SetMusic(Assets::GetInstance().music);
        }
        return music;
    }
    void PlayGame();
    void RestartGame();
    void EndGame();
    void ExitPlayMode();
};