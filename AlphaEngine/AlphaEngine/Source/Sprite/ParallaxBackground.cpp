#include "ParallaxBackground.h"
#include <cmath>
#include <cstdio>   // printf for load warnings

void ParallaxBackground::AddLayer(const char* texturePath,
	float        scrollFactor,
	float        scale)
{
	LayerEntry le;
	le.texture = AEGfxTextureLoad(texturePath);
	le.scrollFactor = scrollFactor;
	le.scale = scale;

	if (!le.texture)
		printf("[ParallaxBackground] WARNING: failed to load '%s'\n", texturePath);

	layers_.push_back(le);
}
void ParallaxBackground::SetAutoScroll(int layerIndex, float speedX, float speedY)
{
	if (layerIndex < 0 || layerIndex >= static_cast<int>(layers_.size()))
		return;
	layers_[layerIndex].autoSpeedX = speedX;
	layers_[layerIndex].autoSpeedY = speedY;
}
void ParallaxBackground::Update(float deltaTime)
{
	for (auto& le : layers_)
	{
		le.offsetX += le.autoSpeedX * deltaTime;
		le.offsetY += le.autoSpeedY * deltaTime;

		// Wrap to avoid float precision loss over long sessions
		if (le.scale > 0.f)
		{
			le.offsetX = fmodf(le.offsetX, le.scale);
			le.offsetY = fmodf(le.offsetY, le.scale);
		}
	}
}
void ParallaxBackground::Clear()
{
	for (auto& le : layers_)
	{
		if (le.texture)
		{
			AEGfxTextureUnload(le.texture);
			le.texture = nullptr;
		}
	}
	layers_.clear();
}
void ParallaxBackground::Draw(AEGfxVertexList* quadMesh,
	float cameraX, float cameraY) const
{
	for (const auto& le : layers_)
		DrawLayer(quadMesh, le, cameraX, cameraY);
}
void ParallaxBackground::DrawLayer(AEGfxVertexList* mesh,
	const LayerEntry& le,
	float cameraX, float cameraY) const
{
	if (!le.texture || !mesh)
		return;

	const float s = le.scale;
	if (s <= 0.f) return;

	float layerX = cameraX * le.scrollFactor + le.offsetX;
	float layerY = cameraY * le.scrollFactor + le.offsetY;

	// Snap to the tile boundary nearest the camera.
	// fmodf gives the sub-tile remainder; subtracting it from
	// the camera position centres the grid on the camera.
	float snapX = cameraX - fmodf(cameraX - layerX, s);
	float snapY = cameraY - fmodf(cameraY - layerY, s);

	// --- Render state (set once per layer) ---
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.f);
	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f);
	AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.f);
	AEGfxTextureSet(le.texture, 0.f, 0.f);

	// --- Draw 3x3 tile grid centred on snapX / snapY ---
	for (int row = -1; row <= 1; ++row)
	{
		for (int col = -1; col <= 1; ++col)
		{
			float wx = snapX + static_cast<float>(col) * s;
			float wy = snapY + static_cast<float>(row) * s;

			AEMtx33Scale(&S, s, s);
			AEMtx33Trans(&T, wx, wy);
			AEMtx33Concat(&M, &T, &S);  // M = T * S

			AEGfxSetTransform(M.m);
			AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
		}
	}
}