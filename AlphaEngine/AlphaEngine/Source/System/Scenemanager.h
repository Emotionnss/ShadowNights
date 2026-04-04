#pragma once

class BaseScene;

/*!
@file       SceneManager.h
@author     Lim Chee Kang (l.cheekang)
@brief      This file contains definitions of functions and variables
             used for the SceneManager class, which handles the lifecycle,
             updating, rendering, and transitioning of different game scenes.
*//*_______________________________________________________________________*/

class SceneManager {
public:
    /*!
    @brief    Queues up a new scene to transition to during the next update loop.
    @param    pNewScene Pointer to the newly allocated scene to load.
    *//*______________________________________________________________*/
    static void ChangeScene(BaseScene* pNewScene);

    /*!
    @brief    Updates the currently active scene and handles the lifecycle
              (Free, Unload, Load, Init) when transitioning between scenes.
    *//*______________________________________________________________*/
    static void Update();

    /*!
    @brief    Calls the specific render function of the currently active scene.
    *//*______________________________________________________________*/
    static void Draw();

    /*!
    @brief    Cleans up and frees memory for the currently active scene
              when the game is shutting down.
    *//*______________________________________________________________*/
    static void Exit();

    /*!
    @brief    Retrieves a pointer to the scene currently being played.
    @return   Pointer to the active BaseScene object.
    *//*______________________________________________________________*/
    static BaseScene* GetCurrentScene() { return pCurrentScene; }

private:
    static BaseScene* pCurrentScene;
    static BaseScene* pNextScene;
};