#pragma once
#include "../Player/GameObject.h"
#include "../System/Sprite.h"
#include <string>

class BaseEnemy : public GameObject
{
public:
    BaseEnemy();
    ~BaseEnemy();

    void Init(float x, float y, float w, float h, std::string spriteName);

    void Update(float deltaTime) override;
    void Draw() override;

private:
    Sprite sprite;
};