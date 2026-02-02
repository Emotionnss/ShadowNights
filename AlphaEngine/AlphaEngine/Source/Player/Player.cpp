#include "Player.h"

Player::Player()
	: input{ 0.f, 0.f }
	, map{ Map::GetInstance() } // Might convert to function for cleaner execution
	, speed{ 4000.f }
	, maxHP{ 100.f }
	, currentHP{ maxHP }
	, maxSpeed{ 800.f }
	, friction{ 0.75f }
	, size{ 70.f }
	, gravity{ -1400.f }
	, isGrounded{ false }
{
	SetStatic(false);

	GetTransform().SetScale(size, size);

	AEGfxGetCamPosition(&cameraPosition.x, &cameraPosition.y);
	AEVec2Set(&cameraOffset, 200.f, -50.f);

	sprite = new Sprite(CSVManager::ReadInfo(SPRITESHEET_DATA, "ame"));
	sprite->SetDefaultAnimation("bounce");
	sprite->PlayDefaultAnimation(-1, 1.f);

	SetAcceleration(4000.f, 4000.f);
}

Player::~Player()
{
	for (BaseProjectile* pProj : projectileList)
	{
		if (pProj)
		{
			delete pProj;
		}
	}
	projectileList.clear();

	if (sprite)
	{
		delete sprite;
		sprite = nullptr;
	}
}

void Player::HandleInput()
{
	AEVec2Zero(&input);

	if (AEInputCheckCurr(AEVK_D))
	{
		Player::input.x += 1.f;
	}

	if (AEInputCheckCurr(AEVK_A))
	{
		Player::input.x -= 1.f;
	}

	if (AEInputCheckTriggered(AEVK_SPACE) && isGrounded)
	{
		SetVelocity(GetVelocity().x, 900.f);
		isGrounded = false;
	}
	else if (!AEInputCheckCurr(AEVK_SPACE) && GetVelocity().y > 0)
	{
		SetVelocity(GetVelocity().x, GetVelocity().y * 0.5f);
	}

	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		s32 xDir, yDir;
		AEInputGetCursorPosition(&xDir, &yDir);
		AEVec2 mousePos = { static_cast<f32>(xDir), static_cast<f32>(yDir) };

		AEVec2 playerDir = ScreenToWorldPos(mousePos);

		projectileList.push_back(new BaseProjectile(GetTransform().GetPosition(), playerDir));
	}
}

void Player::ComputeCollision(AEVec2& position)
{
	AEVec2 halfSize{ size * 0.5f, size * 0.5f };
	AEVec2 oldPosition = GetTransform().GetPosition();

	float left = position.x - halfSize.x;
	float right = position.x + halfSize.x;
	float bottom = position.y - halfSize.y;
	float top = position.y + halfSize.y;

	AEVec2 minTile = map->GetTilePos(left, bottom);
	AEVec2 maxTile = map->GetTilePos(right, top);

	int xMinTile = static_cast<int>((std::min)(minTile.x, maxTile.x));
	int xMaxTile = static_cast<int>((std::max)(minTile.x, maxTile.x));
	int yMinTile = static_cast<int>((std::min)(minTile.y, maxTile.y));
	int yMaxTile = static_cast<int>((std::max)(minTile.y, maxTile.y));

	for (int y = yMinTile; y <= yMaxTile; ++y)
	{
		for (int x = xMinTile; x <= xMaxTile; ++x)
		{
			if (x < 0 || y < 0 || y >= map->GetNumRow() || x >= map->GetNumCol())
				continue;

			Tile& tile = map->GetMap()[y][x];
			if (!map->CheckTile(tile.tileType))
				continue;

			AEVec2 tilePos = tile.transform.GetPosition();
			AEVec2 tileHalf{ map->GetTileSize() * 0.5f, map->GetTileSize() * 0.5f };

			float dx = position.x - tilePos.x;
			float px = (halfSize.x + tileHalf.x) - fabsf(dx);
			if (px <= 0) continue;

			float dy = position.y - tilePos.y;
			float py = (halfSize.y + tileHalf.y) - fabsf(dy);
			if (py <= 0) continue;

			if (tile.tileType == FLOOR)
			{
				float prevBottom = oldPosition.y - halfSize.y;
				float tileTop = tilePos.y + tileHalf.y;

				if (prevBottom >= tileTop && GetVelocity().y <= 0.f)
				{
					position.y = tilePos.y + tileHalf.y + halfSize.y;
					SetVelocity(GetVelocity().x, 0.f);
					isGrounded = true;
				}

				continue;
			}

			if (px < py)
			{
				if ((dx > 0 && GetVelocity().x < 0) || (dx < 0 && GetVelocity().x > 0))
				{
					position.x += (dx > 0) ? px : -px;
					SetVelocity(0.f, GetVelocity().y);
				}
			}
			else
			{
				if ((dy > 0 && GetVelocity().y < 0) || (dy < 0 && GetVelocity().y > 0))
				{
					position.y += (dy > 0) ? py : -py;
					SetVelocity(GetVelocity().x, 0.f);
					if (dy > 0) isGrounded = true;
				}
			}
		}
	}
}

void Player::Update(float deltaTime)
{
	//1. Apply forces / acceleration
	//2. Calculate new velocity
	//3. Calculate new position(based on velocity)
	//4. Check collisions against new position
	//5. Correct position if colliding
	//6. Update transform with corrected position

	HandleInput();

	// Set up forces based on input
	AEVec2 currentForces{ 0.f, gravity }; // Clamp fall speed next time

	if (input.x != 0.f)
	{
		currentForces.x = input.x * speed;
	}
	else
	{
		// Apply friction to velocity directly
		AEVec2 vel = GetVelocity();
		vel.x *= friction;
		SetVelocity(vel.x, vel.y);
	}

	SetAcceleration(currentForces.x, currentForces.y);

	AEVec2 currentPosition = GetTransform().GetPosition();
	AEVec2 currentVelocity = GetVelocity();

	currentVelocity.x += currentForces.x * deltaTime;
	currentVelocity.y += currentForces.y * deltaTime;

	currentVelocity.x = AEClamp(currentVelocity.x, -maxSpeed, maxSpeed);
	SetVelocity(currentVelocity.x, currentVelocity.y);

	AEVec2 predictedPos = currentPosition;
	predictedPos.x += currentVelocity.x * deltaTime;
	predictedPos.y += currentVelocity.y * deltaTime;

	ComputeCollision(predictedPos);

	GetTransform().SetPosition(predictedPos.x, predictedPos.y);

	AEVec2 cameraLerpVector{ GetTransform().GetPosition().x + cameraOffset.x * input.x,
							GetTransform().GetPosition().y + cameraOffset.y };
	AEVec2Lerp(&cameraPosition, &cameraPosition, &cameraLerpVector, deltaTime * 5.f);
	AEGfxSetCamPosition(cameraPosition.x, cameraPosition.y);

	// Update projectiles and remove dead ones
	for (auto it = projectileList.begin(); it != projectileList.end(); )
	{
		(*it)->Update(deltaTime);
		if ((*it)->GetLifespan() <= 0.f)
		{
			delete* it; // Manually delete the object first
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
	matrix.SetPosition(CamPos.x, CamPos.y + 350.f);

	value = AEClamp(value, 0.f, value);
	matrix.SetScale(value * 5.f, 55.f);

	AEGfxSetTransform(matrix.GetMatrix().m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

void Player::Draw(AEGfxVertexList* mesh)
{
	// Draw projectiles first
	for (const auto& p : projectileList)
		p->Draw(mesh);

	/*
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(0.0f);
	AEGfxSetTransform(transform.GetMatrix().m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
	*/

	DrawHealthBar(mesh, currentHP, healthbar);

	// Draw player sprite/transform via GameObject::Draw
	GameObject::Draw();

}
