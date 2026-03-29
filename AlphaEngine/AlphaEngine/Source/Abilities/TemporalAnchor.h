/*!
@file       TemporalAnchor.h
@author     Chew Zavier (c.zavier)
@brief      This file contains declarations of functions used by the Temporal Anchor which is a
            player ability that allows the user to rewind time upon death or reactivation
*//*_______________________________________________________________________*/
#pragma once
#include "../Player/Player.h"
#include "../Sprite/Sprite.h"
#include "../System/AudioManager.h"
#include <vector>
class TemporalAnchor
{
private:
    //a struct for all the ghosts of the players
    struct Ghosts {
        AEVec2 spriteUV;//the sprite UV of the sprite sheet of the players sprite
        AEVec2 position;//position of the ghost
    };
    bool active{};//bool of if the ability is active
    bool abilityAnchoring{};//if the player is anchored
    f32 activeTime{};//timers for the anchor activity
    f32 activeTimer{ activeTime };
    bool spawning{};//the spawning of the anchor
    f32 spawnTime{0.5f};//timer for the spawn animation
    f32 spawnTimer{spawnTime};
    Player* player{};//player pointer
    Transform2D zoneTfm{};//limited area zone transform
    Transform2D anchorTfm{};//anchor tfm
    Transform2D anchorSpawnTfm{};//where the anchor spawning animation players

    Sprite* zoneSprite{ nullptr };//sprite for the zone
    Sprite* anchorSprite{ nullptr };//sprite for the anchor
    Sprite* anchorSpawnSprite{ nullptr };//sprite for the spawning anchor
    bool reversing{};//reversing back in time to the anchor
    size_t iter{ 0 };//iterator for the vector
    f32 savedHealth{};//health when anchoring
    f32 saveTime{ 0.1f };//interval between saving each ghost
    f32 saveTimer{ saveTime };
    std::vector<Ghosts> ghostVect{};//vector fo ghosts

    Transform2D ghostTfm;//ghost transform

    AudioManager* audioManager{};//audio manager

    AEVec2 anchorPos{};//anchor position
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