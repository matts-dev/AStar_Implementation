#include "MoveTypes.h"

const char* Utils::toString(MoveDir direction)
{
	switch (direction)
	{
		case MoveDir::UP:
			return "up";
		case MoveDir::DOWN:
			return "down";
		case MoveDir::LEFT:
			return "left";
		case MoveDir::RIGHT:
			return "right";
		case MoveDir::NONE:
			return "none";
	}
	return "invalid";
}

int Utils::floatToGridLoc(float x)
{
	//int division to get multiple of 32, then upscale back to 32.
	return (lround(x) / 32) * 32;
}

