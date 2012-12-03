#include "statistics.h"

void strikesClass::add(float t, float E)
{
	strikeStruct ss;
	ss.t=t; ss.E=E;
	strikes.push_back (ss);
}
