#include "Player.h"
#include "../Projectiles/BaseProjectile.h"
#include <iostream>
#include "../System/CSVManager.h"
#include <algorithm>
Player::Player()
	: input{ 0.f, 0.f }
	, map {map->GetInstance()}
	, speed{ 500.f }
	, size{ 100.f }
	, gravity{ -1400.f }
	, isGrounded{ false }
{
	GameObject.GetTransform().SetScale(size, size);
	//transform.SetScale(size, size);
	AEGfxGetCamPosition(&cameraPosition.x, &cameraPosition.y);
	AEGfxGetCamPosition(&cameraPosition.x, &cameraPosition.y);
	AEVec2Set(&cameraOffset, 400.f, -50.f);

	GameObject.sprite = new Sprite(CSVManager::ReadInfo(SPRITESHEET_DATA, "ame"));
	GameObject.sprite->PlayAnimation("bounce", -1, 1.f);

	proj = nullptr;
}

Player::~Player()
{
	for (BaseProjectile* pProj : projectileList)
	{
		if (pProj)
			delete pProj;
	}
	projectileList.clear();

	delete GameObject.sprite;
	GameObject.sprite = nullptr;
}

void Player::HandleInput()
{
	// Zero Out Input Vector
	AEVec2Zero(&input);

	if (AEInputCheckCurr(AEVK_D))
	{
		playerDirection = RIGHT;
		input.x += 1.f;
	}

	if (AEInputCheckCurr(AEVK_A))
	{
		playerDirection = LEFT;
		input.x -= 1.f;
	}

	// Jump (Triggered)
	if (AEInputCheckTriggered(AEVK_W) && isGrounded)
	{
		GameObject.SetVelocity(GameObject.GetVelocity().x, 900.f);
		//velocity.y = 900.f;
		isGrounded = false;
	}
	else if (!AEInputCheckCurr(AEVK_W) && GameObject.GetVelocity().y > 0)
	{
		GameObject.SetVelocity(GameObject.GetVelocity().x, GameObject.GetVelocity().y * 0.5f);
		//velocity.y *= 0.5f;
	}

}

void Player::Update(float deltaTime)
{
	const float playerAcceleration = 4000.f;
	const float maxSpeed = 800.f;
	const float friction = 0.75f;
	isGrounded = false;

	if (map == nullptr)
	{
		map = Map::GetInstance();
	}

	//* Gather Input Movement
	if (input.x != 0.f)
	{
		GameObject.SetVelocity(GameObject.GetVelocity().x + input.x * playerAcceleration * deltaTime, GameObject.GetVelocity().y);
	}
	else
	{
		GameObject.SetVelocity(GameObject.GetVelocity().x * friction * deltaTime, GameObject.GetVelocity().y);
	}

	GameObject.SetVelocity(AEClamp(GameObject.GetVelocity().x, -maxSpeed, maxSpeed), GameObject.GetVelocity().y + gravity * deltaTime);

	// Computed Movement
	AEVec2 position = GameObject.GetTransform().GetPosition();
	position.x += GameObject.GetVelocity().x * deltaTime;
	position.y += GameObject.GetVelocity().y * deltaTime;

	AEVec2 halfSize{ size / 2.f, size / 2.f };

	// Min and Max Position of player
	f32 left = position.x - halfSize.x;
	f32 right = position.x + halfSize.x;
	f32 bottom = position.y - halfSize.y;
	f32 top = position.y + halfSize.y;

	// Checks for tiles near player
	AEVec2 minTile = map->GetTilePos(left, bottom);
	AEVec2 maxTile = map->GetTilePos(right, top);

	int xMinTile = static_cast<int>((std::min)(minTile.x, maxTile.x));
	int xMaxTile = static_cast<int>((std::max)(minTile.x, maxTile.x));
	int yMinTile = static_cast<int>((std::min)(minTile.y, maxTile.y));
	int yMaxTile = static_cast<int>((std::max)(minTile.y, maxTile.y));

	// Loops through tile map
	for (int y = yMinTile; y <= yMaxTile; ++y)
	{
		for (int x = xMinTile; x <= xMaxTile; ++x)
		{
			// Out of bounds
			if (x < 0 || y < 0 || y >= map->GetNumRow() || x >= map->GetNumCol())
			{
				continue;
			}

			Tile &tile = map->GetMap()[y][x];

			if (!map->CheckTile(tile.tileType))
			{
				continue;
			}
			else
			{
				std::cout << "Colliding with tile type: " << tile.tileType << std::endl;
			}

			AEVec2 tilePosition = tile.transform.GetPosition();
			AEVec2 tileHalf = { map->GetTileSize() * 0.5f, map->GetTileSize() * 0.5f };

			// Checks between x pos of player and tiles
			float dx = position.x - tilePosition.x;
			float px = (halfSize.x + tileHalf.x) - fabs(dx);

			if (px > 0)
			{
				float dy = position.y - tilePosition.y;
				float py = (halfSize.y + tileHalf.y) - fabs(dy);

				if (py > 0)
				{
					// Resolve smaller axis
					if (px < py)
					{
						if (dx > 0)
						{
							position.x += px;
						}
						else
						{
							position.x += -px;
						}

						GameObject.SetVelocity(0.f, GameObject.GetVelocity().y);
					}
					else
					{
						if (dy > 0)
						{
							position.y += py;
						}
						else
						{
							position.y += -py;
						}
						GameObject.SetVelocity(GameObject.GetVelocity().x, 0.f);

						if (dy > 0 && GameObject.GetVelocity().y <= 0.f)
						{
							isGrounded = true;
						}
					}
				}
			}
		}
	}

	// Grounded Catch
	if (position.y <= -400.f)
	{
		position.y = -400.f;
		GameObject.SetVelocity(GameObject.GetVelocity().x, 0.f);
		isGrounded = true;
	}

	// Apply Physics Movement
	GameObject.GetTransform().SetPosition(position.x, position.y);

	// Apply Camera Movement
	AEVec2 cameraLerpVector{ GameObject.GetTransform().GetPosition().x + cameraOffset.x * input.x,
							GameObject.GetTransform().GetPosition().y + cameraOffset.y };

	// Constant is for tracking speed
	AEVec2Lerp(&cameraPosition, &cameraPosition, &cameraLerpVector, deltaTime * 5.0f);
	AEGfxSetCamPosition(cameraPosition.x, cameraPosition.y);

	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		s32 xDir, yDir;
		AEInputGetCursorPosition(&xDir, &yDir);
		AEVec2 mousePos = { static_cast<f32>(xDir), static_cast<f32>(yDir) };
		AEVec2 playerDir = ScreenToWorldPos(mousePos);

		projectileList.push_back(new BaseProjectile(position, playerDir));
	}
	for (int i = 0; i < projectileList.size(); i++)
	{
		BaseProjectile* pProj = projectileList[i];

		pProj->Update(deltaTime);
		if (pProj->GetLifespan() <= 0.f)
		{
			delete pProj; // free memory
			projectileList.erase(projectileList.begin() + i);
			i--;
		}
	}

	GameObject.sprite->Update();
	//sprite->Update();
}

void Player::Draw(AEGfxVertexList* mesh)
{
	for (BaseProjectile* pProj : projectileList)
	{
		pProj->Draw(mesh);
	}

	/*AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToAdd(-1.f, -1.f, 0.7f, 0.0f);
	AEGfxSetTransform(transform.GetMatrix().m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);*/

	GameObject.sprite->Render(GameObject.GetTransform());
	//sprite->Render(transform);
}
