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
    /*!
    @brief      Default Constructor
    @param      void
    @return     void
    */
    TemporalAnchor();
    /*!
    @brief      Destructor
    @param      void
    @return     void
    */
    ~TemporalAnchor();
    /*!
    @brief      Update
    @param      f32 deltatime
    @return     void
    */
    void Update(f32 dt);
    /*!
    @brief      This function toggles the Temporal Anchor ability , 
                spawning the anchor at the player's position after a short delay
    @param      AEVec2 anchpos - the position to put the anchor at
    @return     void
    */
    bool Anchor(AEVec2 anchPos);
    /*!
    @brief      This function returns if the player has the Temporal Anchor ability is currently active 
    @param      void
    @return     void
    */
    bool Anchor()const;
    /*!
    @brief      This function is responsible for all the rendering of sprites, textures ,animations ,etc.
    @param      void
    @return     void
    */
    void Draw();
    /*!
    @brief      SaveGhost

    This function creates saves the position of the player and the UV of the player's sprite at that instance 
    and pushes it back into the vector

    @param      AEVec2 UVs - The UV of the spritesheet the player is currently showing
                AEVec2 playerPos - the position of the player in that instance
    @return     void
    */
    void SaveGhost(AEVec2 UVs, AEVec2 playerPos);
    /*!
    @brief      This function returns if the player is currently reversing back to the anchor
    @param      void
    @return     void
    */
    bool Reversing() const;
};