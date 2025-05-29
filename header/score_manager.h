#pragma once
#include <raylib.h>
#include "header/ui/score_ui.h"
#include <vector>
#include <algorithm>

class ScoreManager : public IScoreSubject
{
private:
    ScoreManager() {}
    int score = 0;
    int combo = 0;
    int maxCombo = 0;
    std::vector<IScoreObserver *> scoreObservers;

public:
    static ScoreManager &GetInstance()
    {
        static ScoreManager instance;
        return instance;
    }

    // Score
    void AddScore(int amount);
    int GetScore() const { return score; }
    int GetCombo() const { return combo; }
    int GetMaxCombo() const { return maxCombo; }

    void ResetScore()
    {
        score = 0;
        Notify();
    }
    void ResetCombo()
    {
        combo = 0;
        Notify();
    }
    void Reset()
    {
        ResetScore();
        ResetCombo();
        maxCombo = 0;
        Notify();
    }

    // Interface Score Implementation
    void Attach(IScoreObserver *observer) override;
    void Detach(IScoreObserver *observer) override;
    void Notify() override;

    // Delete copy/move
    ScoreManager(ScoreManager const &) = delete;
    void operator=(ScoreManager const &) = delete;
};