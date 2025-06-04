#pragma once
#include "raylib.h"

class IScoreObserver
{
public:
    virtual void OnScoreChanged(int newScore, int newCombo) = 0;
};

class IScoreSubject
{
public:
    virtual void Attach(IScoreObserver *observer) = 0;
    virtual void Detach(IScoreObserver *observer) = 0;
    virtual void Notify() = 0;
    virtual ~IScoreSubject() = default;
};

class ScoreUI : public IScoreObserver
{
private:
    int score = 0;
    int comboCount = 0;
    float comboFontSize = 40;

public:
    void OnScoreChanged(int newScore, int newCombo) override;
    void Draw();
    void DrawCombo();
};