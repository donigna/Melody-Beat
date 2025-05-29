#include "header/score_manager.h"

void ScoreManager::AddScore(int amount)
{
    score += amount;
    combo++;
    if (combo > maxCombo)
    {
        maxCombo = combo;
    }

    Notify();
}

void ScoreManager::Attach(IScoreObserver *observer)
{
    scoreObservers.push_back(observer);
}

void ScoreManager::Detach(IScoreObserver *observer)
{
    scoreObservers.erase(std::remove(scoreObservers.begin(), scoreObservers.end(), observer), scoreObservers.end());
}

void ScoreManager::Notify()
{
    for (IScoreObserver *observer : scoreObservers)
    {
        if (observer)
        {
            observer->OnScoreChanged(score, combo);
        }
    }
}