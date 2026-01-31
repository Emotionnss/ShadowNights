#include "Player.h"
#include "../Projectiles/BaseProjectile.h"
#include "../utils.h"
#include <iostream>
#include "../System/CSVManager.h"


Player::Player()
	: input{ 0.f, 0.f }
	, velocity{ 0.f, 0.f }
	, speed{ 500.f }
	, size{ 50.f }
	, gravity{ -1000.f }
	, isGrounded{ false }
{
	transform.SetScale(size, size);
	AEGfxGetCamPosition(&cameraPosition.x, &cameraPosition.y);
	AEGfxGetCamPosition(&cameraPosition.x, &cameraPosition.y);
	AEVec2Set(&cameraOffset, 400.f, -50.f);

	sprite.Init(CSVManager::ReadSpriteInfo("ame"));

	proj = nullptr;
}

Player::~Player()
{
	for (BaseProjectile* proj : projectileList)
	{
		if (proj)
			delete proj;
	}
	projectileList.clear();
}

void Player::HandleInput()
{
	// Zero Out Input Vector
	AEVec2Zero(&input);

	if (AEInputCheckCurr(AEVK_D)) input.x += 1.f;
	if (AEInputCheckCurr(AEVK_A)) input.x -= 1.f;

	// Jump (Triggered)
	if (AEInputCheckTriggered(AEVK_W) && isGrounded)
	{
		velocity.y = 900.f;
		isGrounded = false;
	}
	else if (!AEInputCheckCurr(AEVK_W) && velocity.y > 0)
	{
		velocity.y *= 0.5f;
	}

}

void Player::Update(float deltaTime)
{
	const float playerAcceleration = 4000.f;
	const float maxSpeed = 800.f;
	const float friction = 0.75f;

	//* Gather Input Movement
	if (input.x != 0.f)
	{
		velocity.x += input.x * playerAcceleration * deltaTime;
	}
	else
	{
		velocity.x *= friction;
	}

	velocity.x = AEClamp(velocity.x, -maxSpeed, maxSpeed);
	velocity.y += gravity * deltaTime;

	//* Compute Physics Movement
	AEVec2 position = transform.GetPosition();
	position.x += velocity.x * deltaTime;
	position.y += velocity.y * deltaTime;

	// Placeholder grounded until I can figure collision
	if (position.y <= -400.f)
	{
		position.y = -400.f;
		velocity.y = 0.f;
		isGrounded = true;
	}

	//* Apply Physics Movement
	transform.SetPosition(position.x, position.y);

	//* Apply Camera Movement
	AEVec2 cameraLerpVector{ transform.GetPosition().x + cameraOffset.x * input.x,
							transform.GetPosition().y + cameraOffset.y };

	// Constant is for tracking speed
	AEVec2Lerp(&cameraPosition, &cameraPosition, &cameraLerpVector, deltaTime * 5.0f);
	AEGfxSetCamPosition(cameraPosition.x, cameraPosition.y);

	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		s32 xDir, yDir;
		AEInputGetCursorPosition(&xDir, &yDir);
		AEVec2 mousePos = {xDir, yDir};
		AEVec2 playerDir = ScreenToWorldPos(mousePos);
		std::cout << playerDir.x << " " << playerDir.y << std::endl;

		projectileList.push_back(new BaseProjectile(position, playerDir));
	}
	for (int i = 0; i < projectileList.size(); i++)
	{
		BaseProjectile* proj = projectileList[i];

		proj->Update(deltaTime);
		if (proj->GetLifespan() <= 0.f)
		{
			delete proj; // free memory
			projectileList.erase(projectileList.begin() + i);
			i--;
		}
	}

	sprite.Update();
}

void Player::Draw(AEGfxVertexList* mesh)
{
	for (BaseProjectile* proj : projectileList)
	{
		proj->Draw(mesh);
	}

	/*AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToAdd(-1.f, -1.f, 0.7f, 0.0f);
	AEGfxSetTransform(transform.GetMatrix().m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);*/

	sprite.Render(transform);
}
