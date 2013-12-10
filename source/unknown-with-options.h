#ifndef _UNKNOWN_WITH_OPTIONS_H_
#define _UNKNOWN_WITH_OPTIONS_H_

class LeftRightLimitsUser
{
protected:
	float left_limit, right_limit;
	bool use_limits;
public:
	LeftRightLimitsUser();
	void setLimits(float left, float right);
	void noLimits();
	bool needContinue(float t);
};

#endif
