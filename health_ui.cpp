#include "header/ui/health_ui.h"

void HealthUI::OnHealthChanged(int newHealth, int maxHealth)
{
    this->health = newHealth;
    this->maxHealth = maxHealth;
}
void HealthUI::Draw()
{
    float ratio = (float)health / maxHealth;
    DrawRectangle(20, 20, 200, 20, GRAY);
    DrawRectangle(20, 20, 200 * ratio, 20, RED);
    DrawText(TextFormat("%d / %d", health, maxHealth), 25, 22, 16, WHITE);
}