#pragma once
#include <vector>

class Extensions
{
public:
	// fast remove for vector which order does not matter
	template<typename T>
	inline static void EraseQuick(std::vector<T>& v, int index)
	{
		if (index >= v.size())
			return;

		if (index != v.size() - 1)
		{
			///swap with last element

			v[index] = v[v.size() - 1];
		}

		// quick erase without realloc
		v.pop_back();
	}

	inline static float Random(float min, float max)
	{
		return std::lerp(min, max, 0.0001f * (rand() % 10000));
	}
};

