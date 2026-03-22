#pragma once
#include "../Player/Player.h"
#include "../Sprite/Sprite.h"
#include "../System/AudioManager.h"
#include <vector>
class TemporalAnchor
{
private:
    u16 row{}, col{};
    struct Ghosts {
        AEVec2 spriteUV;
        AEVec2 position;
    };
    bool active{};
    bool abilityAnchoring{};
    f32 activeTime{};
    f32 activeTimer{ activeTime };
    bool spawning{};
    f32 spawnTime{0.5f};
    f32 spawnTimer{spawnTime};
    Player* player{};
    Transform2D zoneTfm{};
    Transform2D anchorTfm{};
    Transform2D anchorSpawnTfm{};

    Sprite* zoneSprite{ nullptr };
    Sprite* anchorSprite{ nullptr };
    Sprite* anchorSpawnSprite{ nullptr };
    bool reversing{};
    size_t iter{ 0 };
    f32 savedHealth{};
    f32 saveTime{ 0.1f };
    f32 saveTimer{ saveTime };
    std::vector<Ghosts> ghostVect{};

    Transform2D ghostTfm;

    AudioManager* audioManager{};
public:
    TemporalAnchor();
    ~TemporalAnchor();
    void Update(f32 dt);
    bool Anchor(AEVec2 anchPos);
    bool Anchor()const;
    void Draw();
    void SaveGhost(AEVec2 UVs, AEVec2 playerPos);
    bool Reversing() const;
};