#pragma once
#include <cmath>

enum class MoveDir : char { UP, DOWN, LEFT, RIGHT, NONE };

namespace Utils
{
	int floatToGridLoc(float x);
}