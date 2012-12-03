#include "statistics.h"

using namespace std;

void strikesClass::add(float t, float E)
{
	strikeStruct ss;
	ss.t=t; ss.E=E;
	strikes.push_back (ss);
}

int strikesClass::print(string& filename)
{
	return .....
}
