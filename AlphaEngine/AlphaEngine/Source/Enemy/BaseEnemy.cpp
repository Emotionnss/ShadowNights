#include "BaseEnemy.h"
#include "../System/CSVManager.h"

BaseEnemy::BaseEnemy() {
    isStatic = false;
    velocity = { 0.0f, 0.0f };
}

BaseEnemy::~BaseEnemy() {}

void BaseEnemy::Init(float x, float y, float w, float h, std::string spriteName)
{
    transform.SetPosition(x, y);
    transform.SetScale(w, h);

    sprite.Init(CSVManager::ReadSpriteInfo(spriteName));
}

void BaseEnemy::Update(float deltaTime)
{
    GameObject::Update(deltaTime);
    sprite.Update();
}

void BaseEnemy::Draw()
{
    sprite.Render(transform);
}