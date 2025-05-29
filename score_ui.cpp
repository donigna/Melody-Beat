#include "header/ui/score_ui.h"

void ScoreUI::OnScoreChanged(int newScore, int combo)
{
    this->score = newScore;
    this->comboCount = combo;

    comboFontSize = 80;
}

void ScoreUI::Draw()
{
    DrawText(TextFormat("score : %i", score), 20, 50, 15, GREEN);
}

void ScoreUI::DrawCombo()
{
    if (comboCount >= 1)
    {
        if (comboFontSize >= 40)
        {
            comboFontSize -= 0.5f;
        }
        DrawText(TextFormat("x%i", comboCount), GetScreenWidth() / 2 - 50, 40, comboFontSize, WHITE);
    }
}