#include "header/game_manager.h"

void GameManager::MoveToMainMenu()
{
    currentState = MAINMENU;
}

void GameManager::PlayGame()
{
    currentState = PLAYMODE;
    // music.frameCount = 0; // Reset music frame count
    music.looping = true; // Ensure music loops
    SeekMusicStream(music, 6);
    PlayMusicStream(music);
}

void GameManager::RestartGame()
{
    PlayGame();
}

void GameManager::EndGame()
{
    currentState = ENDGAME;
    StopMusicStream(music);
    // UnloadMusicStream(music);
}

void GameManager::ExitPlayMode()
{
    currentState = MAINMENU;
    StopMusicStream(music);
}