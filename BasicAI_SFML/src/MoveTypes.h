#pragma once
#include <cmath>
#include <string>

enum class MoveDir : char { UP, DOWN, LEFT, RIGHT, NONE };

namespace Utils
{
	const char* toString(MoveDir);

	int floatToGridLoc(float x);
}