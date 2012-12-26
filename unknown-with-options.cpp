#include "unknown-with-options.h"

unknownWithOptions::unknownWithOptions(): use_limits(false)
{
}

void unknownWithOptions::setLimits(float left, float right)
{
	left_limit=left;
	right_limit=right;
	use_limits=true;
}

void unknownWithOptions::noLimits()
{
	use_limits=false;
}

bool unknownWithOptions::needContinue(float t)
{
	if (!use_limits) return false;
	if (t < left_limit || t > right_limit) return true;
	return false;
}
