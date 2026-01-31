#include "BaseEnemy.h"
#include <vector>
#include <string> 
BaseEnemy::BaseEnemy() {
    isStatic = false;
    velocity = { -50.0f, 0.0f };
}

BaseEnemy::~BaseEnemy() {}

void BaseEnemy::Init(float x, float y, float w, float h, std::string spriteName, int rows, int cols, int maxFrames)
{
    transform.SetPosition(x, y);
    transform.SetScale(w, h);

    
    std::vector<std::string> spriteData;

    spriteData.push_back(spriteName);
    spriteData.push_back(std::to_string(rows));
    spriteData.push_back(std::to_string(cols));
    spriteData.push_back(std::to_string(maxFrames));

    sprite.Init(spriteData);
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