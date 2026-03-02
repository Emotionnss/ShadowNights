#pragma once
#include "../Player/Player.h"
#include "../Sprite/Sprite.h"
class TemporalAnchor
{
private:
	bool active{};
	f32 activeTime{};
	f32 activeTimer{activeTime};

	Player* player{};
	Transform2D zoneTfm{};
	Transform2D anchorTfm{};

	Sprite* zoneSprite{ nullptr };
	Sprite* anchorSprite{ nullptr };
public:
	TemporalAnchor();
	~TemporalAnchor();
	void Update(f32 dt);
	bool Anchor(AEVec2 anchPos);
	void Draw();
};

