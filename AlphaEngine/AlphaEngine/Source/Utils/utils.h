#pragma once

bool IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
bool IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);
bool AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2);

bool AABBOverlap(AEVec2 aPosition, AEVec2 aHalf, AEVec2 bPosition, AEVec2 bHalf);

AEVec2 ScreenToWorldPos(AEVec2 MouseScreenWorldPos);
