#include "Player.h"

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
{
	SetStatic(false);
	GetTransform().SetScale(size, size); // Set Scale of transform

	AEGfxGetCamPosition(&cameraPosition.x, &cameraPosition.y); // Get Camera Position
	AEVec2Set(&cameraOffset, 200.f, -50.f); // Set Camera Offset Position

	// Sprite Handling
	sprite = new Sprite(CSVManager::ReadInfo("tempPlayer"));
	sprite->SetDefaultAnimation("idle");
	sprite->PlayDefaultAnimation(-1, 1.f);
}

Player::~Player()
{
	// Deleting of protectiles
	for (BaseProjectile* pProj : projectileList)
	{
		if (pProj)
		{
			delete pProj;
		}
	}
	projectileList.clear();

	// Clear Sprite
	if (sprite)
	{
		delete sprite;
		sprite = nullptr;
	}
}

void Player::SetHealth(float val)
{
	currentHP = val;
}

f32 Player::GetHealth()
{
	return currentHP;
}

void Player::SetCollidedXTile(const Tile variable)
{
	collidedXTile = variable;
}

Tile Player::GetCollidedXTile()
{
	return collidedXTile;
}

void Player::SetCollidedYTile(const Tile variable)
{
	collidedYTile = variable;
}
Tile Player::GetCollidedYTile()
{
	return collidedYTile;
}

Sprite* Player::GetSprite() const
{
	return this->sprite;
}

void Player::PlayAnim(std::string name)
{
	if (currentAnimState != name)
	{
		currentAnimState = name;
		if (sprite) sprite->PlayAnimation(name, -1, 1.f);
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
		s32 xDir, yDir;
		AEInputGetCursorPosition(&xDir, &yDir);
		AEVec2 mousePos = { static_cast<f32>(xDir), static_cast<f32>(yDir) };

		AEVec2 playerDir = ScreenToWorldPos(mousePos);
		AEVec2Normalize(&playerDir, &playerDir);

		projectileList.push_back(new BaseProjectile(GetTransform().GetPosition(), playerDir));
	}
	else if (AEInputCheckTriggered(AEVK_RBUTTON))
	{
		s32 xDir, yDir;
		AEInputGetCursorPosition(&xDir, &yDir);
		AEVec2 mousePos = { static_cast<f32>(xDir), static_cast<f32>(yDir) };

		AEVec2 playerDir = ScreenToWorldPos(mousePos);
		AEVec2Normalize(&playerDir, &playerDir);

		projectileList.push_back(new SwordProjectile(GetTransform().GetPosition(), playerDir));
	}
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

			// Y overlap — must overlap on both axes to be touching
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


	const AEVec2 oldPosition = GetTransform().GetPosition(); // Should change this

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

				// Horizontal overlap check — only process if the player is actually above this platform tile
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
			/*
			if (type == TILE_TYPE::FLOOR)
			{
				const float tileTop = tilePos.y + tileHalf.y;
				const float prevBottom = oldPosition.y - halfSize.y;

				// Horizontal overlap
				float dx = position.x - tilePos.x;
				float px = (halfSize.x + tileHalf.x) - fabsf(dx);
				if (px <= 0.f) continue;

				// Vertical overlap
				float dy = position.y - tilePos.y;
				float py = (halfSize.y + tileHalf.y) - fabsf(dy);
				if (py <= 0.f) continue;

				if (prevBottom >= tileTop && GetVelocity().y <= 0.f)
				{
					position.y = tileTop + halfSize.y;
					SetVelocity(GetVelocity().x, 0.f);
					isGrounded = true;
					SetCollidedYTile(tile);
				}
				continue;
			}
			*/

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

void Player::AimRotation()
{
	s32 xDir, yDir;
	AEInputGetCursorPosition(&xDir, &yDir);
	AEVec2 mousePos = { static_cast<f32>(xDir), static_cast<f32>(yDir) };

	AEVec2 playerDir = ScreenToWorldPos(mousePos);
	AEVec2Normalize(&playerDir, &playerDir);

	switch (playerDirection)
	{
	case LEFT:
		if (playerDir.x > 0)
		{
			playerDirection = RIGHT;
			transform.SetScale(-transform.GetScale().x, transform.GetScale().y);
		}
		break;
	case RIGHT:
		if (playerDir.x < 0)
		{
			playerDirection = LEFT;
			transform.SetScale(-transform.GetScale().x, transform.GetScale().y);
		}
		break;
	}
}

void Player::Update(float deltaTime)
{
	HandleInput(); // Handle Input Internally
	AimRotation();
	AEVec2 currentForces{ 0.f, gravity }; // Acceleration

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

	// Collision
	ResolveX(currentPosition);
	currentPosition.y += currentVelocity.y * deltaTime;
	ResolveY(currentPosition);

	// Set Final Position
	GetTransform().SetPosition(currentPosition.x, currentPosition.y);

	AEVec2 cameraLerpVector{ GetTransform().GetPosition().x + cameraOffset.x * input.x,
							GetTransform().GetPosition().y + cameraOffset.y };
	AEVec2 newCameraPos;
	AEVec2Lerp(&newCameraPos, &cameraPosition, &cameraLerpVector, deltaTime * 5.f);
	cameraPosition = newCameraPos;
	AEGfxSetCamPosition(cameraPosition.x, cameraPosition.y);

	// Update Animation FSM
	if (input.x != 0 && isGrounded)
	{
		this->PlayAnim("walk");
	}
	else if (!isGrounded && velocity.y > 0.f)
	{
		this->PlayAnim("jump");
	}
	else if (!isGrounded && velocity.y < 0.f)
	{
		this->PlayAnim("fall");
	}
	else
	{
		this->PlayAnim("idle");
	}

	// Update projectiles and remove dead ones
	for (auto it = projectileList.begin(); it != projectileList.end(); )
	{
		(*it)->Update(deltaTime);
		if ((*it)->GetLifespan() <= 0.f)
		{
			delete* it;
			it = projectileList.erase(it);
		}
		else
			++it;
	}

	if (sprite)
	{
		sprite->Update();
	}
}

void Player::DrawHealthBar(AEGfxVertexList* mesh, float value, Transform2D matrix)
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

	AEVec2 CamPos;
	AEGfxGetCamPosition(&CamPos.x, &CamPos.y);

	matrix.SetRotation(0);
	matrix.SetPosition(CamPos.x - 55.f, CamPos.y + 350.f);
	float safeValue = AEClamp(value, 0.f, maxHP);
	matrix.SetScale(safeValue * 5.f, 55.f);

	AEGfxSetTransform(matrix.GetMatrix().m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

void Player::TakeDamage(float damage)
{
	currentHP -= damage;

	if (currentHP < 0) {
		currentHP = 0;
	}
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

	// Draw player sprite/transform via GameObject::Draw
	GameObject::Draw();
}
