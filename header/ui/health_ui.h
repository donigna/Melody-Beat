#pragma once
#include "raylib.h"

class IHealthObserver
{
public:
    virtual void OnHealthChanged(int newHealth, int maxHealth) = 0;
};

class IHealthSubject
{
public:
    virtual ~IHealthSubject() = default;
    virtual void Attach(IHealthObserver *observer) = 0;
    virtual void Detach(IHealthObserver *observer) = 0;
    virtual void Notify() = 0;
};

class HealthUI : public IHealthObserver
{
private:
    int health = 100;
    int maxHealth = 100;

public:
    void OnHealthChanged(int newHealth, int maxHealth) override;

    void Draw();
};