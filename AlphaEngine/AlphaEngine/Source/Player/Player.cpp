#include "Player.h"

Player::Player()
	: input{ 0.f, 0.f }
	, map{ Map::GetInstance() } // Might convert to function for cleaner execution
	, speed{ 500.f }
	, maxSpeed{ 800.f }
	, friction{ 0.75f }
	, size{ 100.f }
	, gravity{ -1400.f }
	, isGrounded{ false }
{
	SetStatic(false);

	GetTransform().SetScale(size, size);

	AEGfxGetCamPosition(&cameraPosition.x, &cameraPosition.y);
	AEVec2Set(&cameraOffset, 400.f, -50.f);

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
}

void Player::HandleInput()
{
	AEVec2Zero(&input);

	if (AEInputCheckCurr(AEVK_D))
	{
		input.x += 1.f;
	}

	if (AEInputCheckCurr(AEVK_A))
	{
		input.x -= 1.f;
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

void Player::ComputeCollision(AEVec2 position)
{
	AEVec2 halfSize{ size * 0.5f, size * 0.5f };

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
				if (GetVelocity().y < 0.f)
				{
					float prevBottom = position.y - halfSize.y - GetVelocity().y;
					float tileTop = tilePos.y + tileHalf.y;

					if (prevBottom >= tileTop)
					{
						position.y += py;
						SetVelocity(GetVelocity().x, 0.f);
						isGrounded = true;
					}
				}
				continue;
			}

			if (px < py)
			{
				position.x += (dx > 0) ? px : -px;
				SetVelocity(0.f, GetVelocity().y);
			}
			else
			{
				position.y += (dy > 0) ? py : -py;
				SetVelocity(GetVelocity().x, 0.f);
				if (dy > 0) isGrounded = true;
			}
		}
	}
}

void Player::Update(float deltaTime)
{
	// Apply Input Forces
	AEVec2 currentForces{ 0.f, gravity }; // Gravity is applied
	if (input.x != 0.f)
	{
		currentForces.x = input.x * acceleration.x;
	}
	else
	{
		AEVec2 velocity = GetVelocity();
		velocity.x *= friction;
		SetVelocity(velocity.x, velocity.y);
	}

	SetAcceleration(currentForces.x, currentForces.y);

	GameObject::Update(deltaTime);

	AEVec2 position = GetTransform().GetPosition();

	//ComputeCollision(position);

	GetTransform().SetPosition(position.x, position.y);

	AEVec2 cameraLerpVector{ GetTransform().GetPosition().x + cameraOffset.x * input.x,
							GetTransform().GetPosition().y + cameraOffset.y };
	AEVec2Lerp(&cameraPosition, &cameraPosition, &cameraLerpVector, deltaTime * 4.0f);
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
}

void Player::Draw(AEGfxVertexList* mesh)
{
	// Draw projectiles first
	for (const auto& p : projectileList)
		p->Draw(mesh);

	// Draw player sprite/transform via GameObject::Draw
	GameObject::Draw();
}
