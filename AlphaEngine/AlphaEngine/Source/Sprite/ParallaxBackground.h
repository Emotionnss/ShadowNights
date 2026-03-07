#pragma once

#include "AEEngine.h"
#include <vector>

// ============================================================
//  ParallaxBackground
//
//  Manages multiple scrolling background layers.
//  Layers are drawn back-to-front in the order they are added.
//
//  scrollFactor per layer:
//    0.0f = completely fixed to the screen (distant sky)
//    0.5f = moves at half the camera speed  (mid-ground)
//    1.0f = moves 1:1 with the world        (foreground)
//
//  Quick usage:
//
//    // Init/Load
//    bg.AddLayer("assets/sky.png",    0.0f, 1600.f);
//    bg.AddLayer("assets/clouds.png", 0.2f, 1600.f);
//    bg.AddLayer("assets/hills.png",  0.6f, 1600.f);
//    bg.SetAutoScroll(1, 40.f);   // clouds drift rightward
//
//    // Update
//    bg.Update(deltaTime);
//
//    // Draw  (call BEFORE tiles and player)
//    AEVec2 cam; AEGfxGetCamPosition(&cam.x, &cam.y);
//    bg.Draw(quadMesh, cam.x, cam.y);
// ============================================================
class ParallaxBackground
{
public:
	ParallaxBackground() = default;
	~ParallaxBackground() { Clear(); }

	// Load and add a layer.
	//   texturePath  : relative or absolute path accepted by AEGfxTextureLoad
	//   scrollFactor : 0 = fixed, 1 = world speed
	//   scale        : world-space size (width AND height) of one texture copy
	void  AddLayer(const char* texturePath, float scrollFactor, float scale = 1600.f);

	// Give a layer a constant auto-scroll speed (world units / second).
	// Useful for clouds or waterfalls that drift independently of the camera.
	void  SetAutoScroll(int layerIndex, float speedX, float speedY = 0.f);

	// Advance auto-scroll offsets — call once per frame before Draw.
	void  Update(float deltaTime);

	// Draw all layers. Pass the current camera centre in world space.
	// quadMesh must be a unit quad: positions (-0.5,-0.5)-(0.5,0.5),
	// UVs (0,0)-(1,1). This is the standard AEEngine mesh format.
	void  Draw(AEGfxVertexList* quadMesh, float cameraX, float cameraY) const;

	// Unload all textures and reset.
	void  Clear();

	int   LayerCount() const { return static_cast<int>(layers_.size()); }

private:

	struct LayerEntry
	{
		AEGfxTexture* texture{ nullptr };
		float         scrollFactor{ 0.5f };
		float         scale{ 1600.f };
		float         autoSpeedX{ 0.f };
		float         autoSpeedY{ 0.f };
		float         offsetX{ 0.f }; // accumulated auto-scroll X
		float         offsetY{ 0.f }; // accumulated auto-scroll Y
	};

	std::vector<LayerEntry> layers_;

	void DrawLayer(AEGfxVertexList* mesh,
		const LayerEntry& le,
		float cameraX, float cameraY) const;
};
