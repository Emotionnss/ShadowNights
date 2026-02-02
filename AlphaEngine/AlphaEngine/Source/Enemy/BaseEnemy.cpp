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

    sprite = new Sprite(CSVManager::ReadInfo(SPRITESHEET_DATA, spriteName));
    sprite->PlayAnimation("walk", -1, 1.0f);
}

void BaseEnemy::Update(float deltaTime)
{
    GameObject::Update(deltaTime);
}

void BaseEnemy::Draw()
{
    GameObject::Draw();
}