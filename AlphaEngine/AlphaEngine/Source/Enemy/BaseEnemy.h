#pragma once
#include "../Player/GameObject.h"
#include "../Sprite/Sprite.h"
#include <string>

class BaseEnemy : public GameObject
{
public:
    BaseEnemy();
    ~BaseEnemy();

    void Init(float x, float y, float w, float h, std::string spriteName);

    void Update(float deltaTime) override;
    void Draw() override;
};