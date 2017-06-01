#include "UtilityFunctions.h"

const float JakePerry::Clampf(const float value, const float min, const float max)
{
	if (value > max) return max;
	if (value < min) return min;
	return value;
}

const double JakePerry::Clampd(const double value, const double min, const double max)
{
	return 0.0;
}
