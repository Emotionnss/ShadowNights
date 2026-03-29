#include <math.h>
#include "AEEngine.h"
#include "utils.h"

bool IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	AEVec2 xLimits = { area_center_x - area_width / 2, area_center_x + area_width / 2 }, yLimits = { area_center_y - area_height / 2, area_center_y + area_height / 2 };

	if (click_x >= xLimits.x && click_x <= xLimits.y && click_y >= yLimits.x && click_y <= yLimits.y)
	{
		return true;
	}

	return false;
}

bool IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y)
{
	AEVec2 circleCenter = { circle_center_x, circle_center_y }, clickPos = { click_x, click_y };
	float distance = AEVec2Distance(&circleCenter, &clickPos);

	float distanceSquared = distance * distance;
	float radiusSquared = (diameter / 2) * (diameter / 2);

	if (distanceSquared < radiusSquared)
	{
		return true;
	}

	return false;
}

bool AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2)
{
	AEVec2 circle1Center = { c1_x, c1_y }, circle2Center = { c2_x, c2_y };

	float distance = AEVec2Distance(&circle1Center, &circle2Center);
	float distanceSquared = distance * distance;

	float combinedRadius = r1 + r2;
	float combinedRadiusSquared = combinedRadius * combinedRadius;

	if (distanceSquared < combinedRadiusSquared)
	{
		return true;
	}

	return false;
}

bool AABBOverlap(AEVec2 aPosition, AEVec2 aHalf, AEVec2 bPosition, AEVec2 bHalf)
{
	return fabs(aPosition.x - bPosition.x) <= (aHalf.x + bHalf.x) &&
		fabs(aPosition.y - bPosition.y) <= (aHalf.y + bHalf.y);
}

AEVec2 ScreenToWorldPos(AEVec2 MouseScreenWorldPos)
{
	AEVec2 temp{};
    temp.x = MouseScreenWorldPos.x - (AEGfxGetWindowWidth() * 0.5f);
    temp.y = (AEGfxGetWindowHeight() * 0.5f) - MouseScreenWorldPos.y;
    return temp;
}
