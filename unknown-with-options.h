#ifndef _UNKNOWN_WITH_OPTIONS_H_
#define _UNKNOWN_WITH_OPTIONS_H_

class unknownWithOptions
{
protected:
	float left_limit, right_limit;
	bool use_limits;
public:
	unknownWithOptions();
	void setLimits(float left, float right);
	void noLimits();
};

#endif
