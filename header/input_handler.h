#pragma once
#include "command.h"
#include "header/game_manager.h"

class PlayCommand : public ICommand
{
public:
    void Execute() override
    {
        GameManager::GetInstance().PlayGame();
    }
};

class RestartCommand : public ICommand
{
public:
    void Execute() override
    {
        GameManager::GetInstance().RestartGame();
    }
};

class ExitPlayCommand : public ICommand
{
public:
    void Execute() override
    {
        GameManager::GetInstance().ExitPlayMode();
    }
};