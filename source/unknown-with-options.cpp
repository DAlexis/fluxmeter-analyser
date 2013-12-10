#include "unknown-with-options.h"

LeftRightLimitsUser::LeftRightLimitsUser():
	use_limits(false)
{
}

void LeftRightLimitsUser::setLimits(float left, float right)
{
	left_limit=left;
	right_limit=right;
	use_limits=true;
}

void LeftRightLimitsUser::noLimits()
{
	use_limits=false;
}

bool LeftRightLimitsUser::needContinue(float t)
{
	if (!use_limits) return false;
	if (t < left_limit || t > right_limit) return true;
	return false;
}
