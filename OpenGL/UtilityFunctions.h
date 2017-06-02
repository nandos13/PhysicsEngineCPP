#pragma once

#include <list>

namespace JakePerry
{
	const float Clampf(const float value, const float min, const float max);
	const double Clampd(const double value, const double min, const double max);

	template <class T>
	const std::list<T> AppendList(const std::list<T> listA, const std::list<T> listB)
	{
		std::list<T> returnList;
		for (auto& iter = listA.cbegin(); iter != listA.cend(); iter++)
		{
			returnList.push_back(*iter);
		}
		for (auto& iter = listB.cbegin(); iter != listB.cend(); iter++)
		{
			returnList.push_back(*iter);
		}

		return returnList;
	}
}