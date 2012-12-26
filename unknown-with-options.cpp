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
