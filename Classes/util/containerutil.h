#pragma once
#include <vector>
#include <map>

namespace BMS
{
	template <typename T1, typename T2>
	void clearMap(std::map<T1, T2>& map)
	{
		map.swap(std::map<T1, T2>());
	}

	template <typename T1, typename T2>
	void clearPointerMap(std::map<T1, T2>& map)
	{
		for(auto it = map.begin(); it != map.end(); it++)
			delete it->second;

		clearMap(map);
	}

	template <typename T1>
	void clearVector(std::vector<T1>& vector)
	{
		vector.swap(std::vector<T1>());
	}

	template <typename T1>
	void clearPointerVector(std::vector<T1>& vector)
	{
		for(auto it = vector.begin(); it != vector.end(); it++)
			delete *it;

		clearVector(vector);
	}
}