#include "BossExplosive.h"
#include <iostream>

BossExplosive::BossExplosive()
{

}

BossExplosive::~BossExplosive()
{

}

bool BossExplosive::Triggered(AEVec2 pos, AEVec2 dir)
{
	std::cout << "EXPLODING GRAHHHH" << std::endl;

	return true;
}

void BossExplosive::Explode(AEVec2 origin)
{
}