#pragma once
#include "../Player/Player.h"
#include "../Sprite/Sprite.h"
#include <vector>
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

	bool reversing{};
	size_t iter{ 0 };
	f32 savedHealth{};
	f32 saveTime{ 0.1f };
	f32 saveTimer{ saveTime };
	std::vector<AEVec2> savedPlayerPositions{};
public:
	TemporalAnchor();
	~TemporalAnchor();
	void Update(f32 dt);
	bool Anchor(AEVec2 anchPos);
	bool Anchor()const;
	void Draw();
	void SavePositions(AEVec2 playerPos);
	bool Reversing() const;
};

