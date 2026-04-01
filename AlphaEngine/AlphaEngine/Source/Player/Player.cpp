#include "Player.h"
#include "../particle/ParticleManager.h"
#include <iostream>

Player::Player()
	: input{ 0.f, 0.f }
	, map{ Map::GetInstance() }
	, collidedXTile{ Tile{} }
	, collidedYTile{ Tile{} }
	, speed{ 4000.f }
	, maxHP{ 100.f }
	, currentHP{ maxHP }
	, maxSpeed{ 800.f }
	, friction{ 0.75f }
	, size{ 70.f }
	, jumpForce{ 1000.f }
	, gravity{ -1400.f }
	, isGrounded{ false }
	, damagedTimer{ 0.f }
	, healthBarBG{ nullptr }
	, playerToMouseVector{ 0.f, 0.f }
	, playerToMouseVectorNormalized{ 0.f, 0.f }
	, cameraOffset{ 0.f, 0.f }
	, shakeIntensity{ 0.f }
	, shakeDuration{ 0.f }
	, shakeTimer{ 0.f }
{
	SetStatic(false);
	GetTransform().SetScale(size, size); // Set Scale of transform

	AEGfxGetCamPosition(&cameraPosition.x, &cameraPosition.y); // Get Camera Position
	//AEVec2Set(&cameraOffset, 200.f, -50.f); // Set Camera Offset Position

	healthbarOverlay = AEGfxTextureLoad("Assets/Abilities/healthbar.png");
	healthbarOverlayTfm.SetScale(maxHP * 5.f, 100.f);
	arrowTex = AEGfxTextureLoad("Assets/Weapons/pointer.png");
	// Sprite Handling
	sprite = new Sprite(CSVManager::ReadInfo("tempPlayer"));
	sprite->SetDefaultAnimation("idle");
	sprite->PlayDefaultAnimation(-1, 1.f);
	AudioManager::GetInstance()->PlayEffect("player_move", 0.98f, 0.5f);
	arrowTfm.SetScale(25, 25);
}

Player::~Player()
{
	ClearProjectileList();

	// Clear Sprite
	if (sprite)
	{
		delete sprite;
		sprite = nullptr;
	}
	if (healthbarOverlay)
		AEGfxTextureUnload(healthbarOverlay);
	if (arrowTex)
		AEGfxTextureUnload(arrowTex);
}

void Player::SetHealth(float val)
{
	currentHP = val;
}

f32 Player::GetHealth() const
{
	return currentHP;
}

void Player::SetCollidedXTile(const Tile variable)
{
	collidedXTile = variable;
}

Tile Player::GetCollidedXTile() const
{
	return collidedXTile;
}

void Player::SetCollidedYTile(const Tile variable)
{
	collidedYTile = variable;
}
Tile Player::GetCollidedYTile() const
{
	return collidedYTile;
}

Sprite* Player::GetSprite() const
{
	return this->sprite;
}

void Player::PlayAnim(std::string name, s32 count)
{
	if (currentAnimState != name)
	{
		currentAnimState = name;
		if (sprite)
		{
			sprite->PlayAnimation(name, count, 1.f);
		}
	}
}

void Player::HandleInput()
{
	AEVec2Zero(&input);

	if (AEInputCheckCurr(AEVK_D))
	{
		this->input.x += 1.f;
	}

	if (AEInputCheckCurr(AEVK_A))
	{
		this->input.x -= 1.f;
	}

	// Jumping
	if (AEInputCheckTriggered(AEVK_SPACE) && isGrounded)
	{
		SetVelocity(GetVelocity().x, jumpForce);
		isGrounded = false;
	}
	else if (!AEInputCheckCurr(AEVK_SPACE) && GetVelocity().y > 0)
	{
		SetVelocity(GetVelocity().x, GetVelocity().y * 0.5f);
	}

	// Attacking
	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		AudioManager::GetInstance()->PlaySFXOneShot("player_attack");

		projectileList.push_back(new BaseProjectile(GetTransform().GetPosition(), playerToMouseVectorNormalized));
	}
	else if (AEInputCheckTriggered(AEVK_RBUTTON))
	{
		if (swordCooldown <= 0.f)
		{
			AudioManager::GetInstance()->PlaySFXOneShot("player_slash");

			projectileList.push_back(new SwordProjectile(GetTransform().GetPosition(), playerToMouseVectorNormalized));
			swordCooldown = swordMaxCooldown;
		}
	}
}

AEVec2 Player::CalcPlayerToMouseDirNormalized(AEVec2& output, AEVec2& cameraOffsetValue)
{
	// I've refactored this part 6 times please no more
	s32 xDir, yDir;
	AEInputGetCursorPosition(&xDir, &yDir);
	AEVec2 mouseScreenPos = { static_cast<f32>(xDir), static_cast<f32>(yDir) };

	// Alienise output for direction and aim from camera offset
	AEVec2 mouseViewportPos = ScreenToWorldPos(mouseScreenPos);
	cameraOffsetValue.x = mouseViewportPos.x;
	cameraOffsetValue.y = mouseViewportPos.y;

	AEVec2 mouseWorldPos{};
	mouseWorldPos.x = mouseViewportPos.x + cameraPosition.x;
	mouseWorldPos.y = mouseViewportPos.y + cameraPosition.y;

	AEVec2 playerPos = transform.GetPosition();
	playerToMouseVector.x = mouseWorldPos.x - playerPos.x;
	playerToMouseVector.y = mouseWorldPos.y - playerPos.y;

	output = playerToMouseVector;
	AEVec2Normalize(&output, &output);
	return output;
}

void Player::AimArrow(AEVec2& inputValue)
{
	arrowTfm.SetRotation(atan2f(inputValue.y, inputValue.x));
	arrowTfm.SetPosition(transform.GetPosition().x + inputValue.x * 50, transform.GetPosition().y + inputValue.y * 50);
}

void Player::ResolveX(AEVec2& position)
{
	const AEVec2 halfSize{ size * 0.5f, size * 0.5f };
	const float  tileSizeHalf = map->GetTileSize() * 0.5f;
	const AEVec2 tileHalf{ tileSizeHalf, tileSizeHalf };

	AEVec2 minTile = map->GetTilePos(position.x - halfSize.x, position.y - halfSize.y);
	AEVec2 maxTile = map->GetTilePos(position.x + halfSize.x, position.y + halfSize.y);

	int xMin = static_cast<int>((std::min)(minTile.x, maxTile.x));
	int xMax = static_cast<int>((std::max)(minTile.x, maxTile.x));
	int yMin = static_cast<int>((std::min)(minTile.y, maxTile.y));
	int yMax = static_cast<int>((std::max)(minTile.y, maxTile.y));

	for (int y = yMin; y <= yMax; ++y)
	{
		for (int x = xMin; x <= xMax; ++x)
		{
			if (x < 0 || y < 0 || y >= map->GetNumRow() || x >= map->GetNumCol())
				continue;

			Tile& tile = map->GetMap()[y][x];
			if (!map->CheckTile(tile.tileType))
				continue;

			TILE_TYPE type = tile.tileType;

			// Ignore tile if it's platform
			if (type == TILE_TYPE::BACKGROUND ||
				type == TILE_TYPE::LEFT_PLATFORM ||
				type == TILE_TYPE::PLATFORM ||
				type == TILE_TYPE::RIGHT_PLATFORM)
				continue;

			AEVec2 tilePos = tile.transform.GetPosition();

			// X overlap
			float dx = position.x - tilePos.x;
			float px = (halfSize.x + tileHalf.x) - fabsf(dx);
			if (px <= 0.f) continue;

			// Y overlap � must overlap on both axes to be touching
			float dy = position.y - tilePos.y;
			float py = (halfSize.y + tileHalf.y) - fabsf(dy);
			if (py <= 0.f) continue;

			SetCollidedXTile(tile);

			// Push Out Player
			if ((dx > 0.f && GetVelocity().x < 0.f) ||
				(dx < 0.f && GetVelocity().x > 0.f))
			{
				position.x += (dx > 0.f) ? px : -px;
				SetVelocity(0.f, GetVelocity().y);
			}
		}
	}
}

void Player::ResolveY(AEVec2& position)
{
	const AEVec2 halfSize{ size * 0.5f, size * 0.5f };
	const float  tileSizeHalf = map->GetTileSize() * 0.5f;
	const AEVec2 tileHalf{ tileSizeHalf, tileSizeHalf };

	const AEVec2 oldPosition = GetTransform().GetPosition();

	isGrounded = false;

	AEVec2 minTile = map->GetTilePos(position.x - halfSize.x, position.y - halfSize.y);
	AEVec2 maxTile = map->GetTilePos(position.x + halfSize.x, position.y + halfSize.y);

	int xMin = static_cast<int>((std::min)(minTile.x, maxTile.x));
	int xMax = static_cast<int>((std::max)(minTile.x, maxTile.x));
	int yMin = static_cast<int>((std::min)(minTile.y, maxTile.y));
	int yMax = static_cast<int>((std::max)(minTile.y, maxTile.y));

	for (int y = yMin; y <= yMax; ++y)
	{
		for (int x = xMin; x <= xMax; ++x)
		{
			if (x < 0 || y < 0 || y >= map->GetNumRow() || x >= map->GetNumCol())
				continue;

			Tile& tile = map->GetMap()[y][x];
			TILE_TYPE type = tile.tileType;
			if (!map->CheckTile(tile.tileType) || type == TILE_TYPE::BACKGROUND)
				continue;

			AEVec2 tilePos = tile.transform.GetPosition();

			// One way platform
			if (type == TILE_TYPE::LEFT_PLATFORM ||
				type == TILE_TYPE::PLATFORM ||
				type == TILE_TYPE::RIGHT_PLATFORM)
			{
				// Surface sits at the tile's top edge
				const float surfaceY = tilePos.y + tileSizeHalf;
				const float prevBottom = oldPosition.y - halfSize.y;
				const float newBottom = position.y - halfSize.y;

				// Horizontal overlap check � only process if the player is actually above this platform tile
				float dx = position.x - tilePos.x;
				float px = (halfSize.x + tileHalf.x) - fabsf(dx);
				if (px <= 0.f) continue;

				// Land only when:
				//   feet were above the surface last frame
				//   player is falling
				//   feet have reached or crossed the surface this frame
				if (prevBottom >= surfaceY &&
					GetVelocity().y <= 0.f &&
					newBottom <= surfaceY)
				{
					position.y = surfaceY + halfSize.y;
					SetVelocity(GetVelocity().x, 0.f);
					isGrounded = true;
					SetCollidedYTile(tile);
				}
				continue;
			}

			// Tile Resolution
			float dx = position.x - tilePos.x;
			float px = (halfSize.x + tileHalf.x) - fabsf(dx);
			if (px <= 0.f) continue;

			float dy = position.y - tilePos.y;
			float py = (halfSize.y + tileHalf.y) - fabsf(dy);
			if (py <= 0.f) continue;

			SetCollidedYTile(tile);

			// Push out player
			if ((dy > 0.f && GetVelocity().y < 0.f) ||
				(dy < 0.f && GetVelocity().y > 0.f))
			{
				position.y += (dy > 0.f) ? py : -py;
				SetVelocity(GetVelocity().x, 0.f);
				if (dy > 0.f) isGrounded = true;
			}
		}
	}
}

void Player::AimRotation(AEVec2& inputValue)
{
	switch (playerDirection)
	{
	case LEFT:
		if (inputValue.x > 0)
		{
			playerDirection = RIGHT;
			transform.SetScale(-transform.GetScale().x, transform.GetScale().y);
		}
		break;
	case RIGHT:
		if (inputValue.x < 0)
		{
			playerDirection = LEFT;
			transform.SetScale(-transform.GetScale().x, transform.GetScale().y);
		}
		break;
	}
}

void Player::TriggerCameraShake(f32 intensity, f32 duration)
{
	shakeIntensity = intensity;
	shakeDuration = duration;
	shakeTimer = duration;
}

void Player::UpdateCameraPosition(f32 _dt)
{
	AEVec2 playerPosition = GetTransform().GetPosition();

	// Clamps offset between player and cursor to certain lengths
	float clampedOffsetX = AEClamp(cameraOffset.x, -200.f, 200.f);
	float clampedOffsetY = AEClamp(cameraOffset.y, -150.f, 150.f);

	AEVec2 cameraLerpTarget{
		playerPosition.x + clampedOffsetX,
		playerPosition.y + clampedOffsetY
	};

	AEVec2Lerp(&cameraPosition, &cameraPosition, &cameraLerpTarget, _dt * 4.f);

	// Gather minBound tile and maxBound tile
	Tile minTile = map->GetTileFromTilePos(static_cast<float>(0), static_cast <float>(0));
	Tile maxTile = map->GetTileFromTilePos(static_cast<float>(map->GetNumCol() - 1), static_cast<float>(map->GetNumRow() - 1));

	// Gather tiles world position
	AEVec2 minBound = minTile.transform.GetPosition();
	AEVec2 maxBound = maxTile.transform.GetPosition();

	float halfTileSize = map->GetTileSize() / 2;

	// Ensure min is actually less than max
	float minX = (minBound.x < maxBound.x) ? minBound.x : maxBound.x;
	float maxX = (maxBound.x > minBound.x) ? maxBound.x : minBound.x;
	float minY = (minBound.y < maxBound.y) ? minBound.y : maxBound.y;
	float maxY = (maxBound.y > minBound.y) ? maxBound.y : minBound.y;

	// Account for camera viewport size
	float halfViewWidth = AEGfxGetWindowWidth() * 0.5f;
	float halfViewHeight = AEGfxGetWindowHeight() * 0.5f;

	// Clamp camera position to map bounds minus half viewport
	cameraPosition.x = AEClamp(cameraPosition.x, minX + halfViewWidth - halfTileSize, maxX - halfViewWidth + halfTileSize);
	cameraPosition.y = AEClamp(cameraPosition.y, minY + halfViewHeight - halfTileSize, maxY - halfViewHeight + halfTileSize);

	if (shakeTimer > 0.f)
	{
		shakeTimer -= _dt;
		float progress = shakeTimer / shakeDuration;
		float currentIntensity = shakeIntensity * progress;

		float offsetX = (AERandFloat() * 2.f - 1.f) * currentIntensity;
		float offsetY = (AERandFloat() * 2.f - 1.f) * currentIntensity;

		if (!SettingsManager::GetInstance()->GetCameraShake())
		{
			return;
		}

		cameraPosition.x += offsetX;
		cameraPosition.y += offsetY;
	}

	AEGfxSetCamPosition(cameraPosition.x, cameraPosition.y);
}

void Player::Update(float deltaTime)
{
	if (damagedTimer > 0.f)
	{
		damagedTimer -= deltaTime;
	}

	CalcPlayerToMouseDirNormalized(playerToMouseVectorNormalized, cameraOffset); // Calculates player to mouse direction normalized

	if (canMove)
	{
		HandleInput();
	}
	else
	{
		input.x = 0;
	}

	AEVec2 currentForces{ 0.f, gravity }; // Acceleration

	if (swordCooldown >= 0.f)
	{
		swordCooldown -= deltaTime;
	}

	// Check For X Input
	if (input.x != 0.f)
	{
		currentForces.x = input.x * speed;
	}
	else
	{
		SetVelocity(GetVelocity().x * friction, GetVelocity().y);
	}

	// Update Current Velocity
	AEVec2 currentVelocity = GetVelocity();
	currentVelocity.x += currentForces.x * deltaTime;
	currentVelocity.y += currentForces.y * deltaTime;
	currentVelocity.x = AEClamp(currentVelocity.x, -maxSpeed, maxSpeed); // Apply y velocity later if needed
	SetVelocity(currentVelocity.x, currentVelocity.y);

	// Update Current Position
	AEVec2 currentPosition = GetTransform().GetPosition();
	currentPosition.x += currentVelocity.x * deltaTime;
	ResolveX(currentPosition);
	currentPosition.y += currentVelocity.y * deltaTime;
	ResolveY(currentPosition);

	// Set Final Position
	GetTransform().SetPosition(currentPosition.x, currentPosition.y);

	AimArrow(playerToMouseVectorNormalized); // Updates Aim Arrow after player position
	AimRotation(playerToMouseVectorNormalized); // Checks player facing direction

	//Camera Update
	UpdateCameraPosition(deltaTime);

	// Update Animation FSM
	if (input.x != 0 && isGrounded)
	{
		bool isFacingInputDirection = (input.x > 0) == (playerDirection == Direction::RIGHT);
		this->PlayAnim(isFacingInputDirection ? "walk_forward" : "walk_backward");
		AudioManager::GetInstance()->ResumeEffect();//continue walking sound
		//AudioManager::GetInstance()->PlaySFXOneShot("player_move");

	}
	else if (!isGrounded && velocity.y > 0.f)
	{
		this->PlayAnim("jump");

		// --- FIX 3: Smaller, Grey jump dust so it doesn't blind the screen! ---
		ParticleManager::GetInstance()->SpawnTrail(
			GetTransform().GetPosition(), { 30.f, 30.f }, 0.2f, 0.5f, 0.5f, 0.5f
		);
		AudioManager::GetInstance()->PauseEffect();//pause walking sound	
	}
	else if (!isGrounded && velocity.y < 0.f)
	{
		AudioManager::GetInstance()->PauseEffect();//pause walking sound
		this->PlayAnim("fall");
	}
	else
	{
		this->PlayAnim("idle");
		AudioManager::GetInstance()->PauseEffect();//pause walking sound
	}

	if (sprite)
	{
		sprite->Update();
	}

	// Update projectiles and remove dead ones
	for (auto it = projectileList.begin(); it != projectileList.end(); )
	{
		(*it)->Update(deltaTime);

		if (CheckProjectileCollision(*it, map))
			(*it)->SetLifespan(-1.f);

		if ((*it)->GetLifespan() <= 0.f)
		{
			delete* it;
			it = projectileList.erase(it);
		}
		else
			++it;
	}
}

void Player::DrawHealthBar(AEGfxVertexList* mesh, float value, Transform2D matrix)
{
	if (canMove)
	{
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(0.25f);

		AEGfxSetColorToMultiply(1.f, 0.0f, 0.0f, 1.0f);
		AEGfxSetColorToAdd(1.f, 0.f, 0.f, 1.f);

		if (AEInputCheckTriggered(AEVK_1))
		{
			TakeDamage(10);
		}

		AEVec2 CamPos{};
		AEGfxGetCamPosition(&CamPos.x, &CamPos.y);
		matrix.SetPosition(CamPos.x - AEGfxGetWindowWidth() / 2.f + 168.f, CamPos.y + 335.f);
		float safeValue = AEClamp(value, 0.f, maxHP);
		matrix.SetScale(safeValue * 3.75f, 25.f);

		healthbarOverlayTfm.SetPosition(CamPos.x - AEGfxGetWindowWidth() / 2.f + 50.f, CamPos.y + 300.f);

		AEGfxSetTransform(matrix.GetMatrix().m);
		AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.0f);

		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.f);

		AEGfxTextureSet(healthbarOverlay, 0, 0);
		AEGfxSetTransform(healthbarOverlayTfm.GetMatrix().m);
		AEGfxMeshDraw(MeshManager::GetInstance()->GetMesh(CORNER_TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);
	}
}

void Player::TakeDamage(float damage)
{
	AudioManager::GetInstance()->PlaySFXOneShot("player_hurt");

	float x = (currentHP - damage < 0) ? 0 : (currentHP - damage);
	SetHealth(x);
	damagedTimer = 0.5f;
}

void Player::ClearProjectileList()
{
	for (BaseProjectile* pProj : projectileList)
	{
		if (pProj)
		{
			delete pProj;
		}
	}

	projectileList.clear();
}

void Player::Draw(AEGfxVertexList* mesh)
{
	// Draw projectiles first
	for (const auto& p : projectileList)
		p->Draw(mesh);

	if (SettingsManager::GetInstance()->GetPlayerColDebug())
	{
		AEGfxVertexList* squareMesh = MeshManager::GetInstance()->GetMesh(SQUARE);

		// Draw For X Collision
		if (collidedXTile.tileType != TILE_TYPE::NONE)
		{
			Transform2D tileXTransform{ GetCollidedXTile().transform };
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);

			AEGfxSetColorToAdd(0.25f, 0.75f, 0.25f, 1.f);
			AEGfxSetColorToMultiply(0.0f, 0.5f, 0.0f, 1.f);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);

			//Create transform

			tileXTransform.SetScale(tileXTransform.GetScale().x + 10.f, tileXTransform.GetScale().y + 10.f);

			AEGfxSetTransform(tileXTransform.GetMatrix().m);
			AEGfxMeshDraw(squareMesh, AE_GFX_MDM_TRIANGLES);
		}

		// Draw For Y Collision
		if (collidedYTile.tileType != TILE_TYPE::NONE)
		{
			//AEGfxVertexList* squareMesh = MeshManager::GetInstance()->GetMesh(SQUARE);
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);

			AEGfxSetColorToAdd(0.25f, 0.75f, 0.25f, 1.f);
			AEGfxSetColorToMultiply(0.0f, 0.5f, 0.0f, 1.f);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);

			//Create transform
			Transform2D tileYTransform{ GetCollidedYTile().transform };

			tileYTransform.SetScale(tileYTransform.GetScale().x + 10.f, tileYTransform.GetScale().y + 10.f);

			AEGfxSetTransform(tileYTransform.GetMatrix().m);
			AEGfxMeshDraw(squareMesh, AE_GFX_MDM_TRIANGLES);
		}


	}

	DrawHealthBar(mesh, currentHP, healthbar);

	// Change to draw in player.cpp
	if (sprite != nullptr)
	{
		if (damagedTimer > 0.f)
		{
			sprite->Render(transform, 255.f, 1.f, 1.f, 1.f);
		}
		else
		{
			sprite->Render(transform);

		}
	}
	//for arrow surrounding player
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	if (swordCooldown <= 0.f)
		AEGfxSetColorToAdd(0.24f, 1.f, 0.84f, 0.0f);
	else
		AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.0f);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1);

	AEGfxTextureSet(arrowTex, 0, 0);
	AEGfxSetTransform(arrowTfm.GetMatrix().m);
	AEGfxMeshDraw(MeshManager::GetInstance()->GetMesh(TEXTURE_SQUARE), AE_GFX_MDM_TRIANGLES);
	/*
	AEGfxTextureSet(nullptr, 0, 0);
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f);


	AEMtx33 identity;
	AEMtx33Identity(&identity);
	AEGfxSetTransform(identity.m);
	*/
}

bool Player::CheckProjectileCollision(BaseProjectile* proj, Map* mapData)
{
	for (s32 i = 0; i < mapData->GetNumCol(); ++i)
		for (s32 j = 0; j < mapData->GetNumRow(); ++j)
		{
			Tile const& tmp = mapData->GetTileFromTilePos(static_cast<float>(i), static_cast<float>(j));
			AEVec2 projHalf = { proj->transform.GetScale().x * 0.5f, proj->transform.GetScale().y * 0.5f };
			AEVec2 tileHalf = { tmp.transform.GetScale().x * 0.5f, tmp.transform.GetScale().y * 0.5f };

			if (AABBOverlap(proj->transform.GetPosition(), projHalf, tmp.transform.GetPosition(), tileHalf) && proj->collidesWithTiles)
				return mapData->CheckTile(tmp.tileType);
		}
	return false;
}

void Player::SetMoveable(bool state)
{
	canMove = state;
}
