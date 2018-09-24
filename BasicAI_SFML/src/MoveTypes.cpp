#include "MoveTypes.h"

int Utils::floatToGridLoc(float x)
{
	//int division to get multiple of 32, then upscale back to 32.
	return (lround(x) / 32) * 32;
}

