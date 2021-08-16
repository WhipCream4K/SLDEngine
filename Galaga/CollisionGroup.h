#pragma once

enum CollisionGroup : uint16_t
{
	mPlayer = 1,
	Enemy = 1 << 1,
	Projectile = 1 << 3
};