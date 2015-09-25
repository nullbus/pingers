#pragma once
#include <vector>
#include <map>

namespace BMS
{
	template <typename K, typename V>
	void clearMap(std::map<K, V>& map)
	{
		std::map<K, V> empty;
		map.swap(empty);
	}

	template <typename K, typename V>
	void clearPointerMap(std::map<K, V>& map)
	{
		for(auto it = map.begin(); it != map.end(); it++)
			delete it->second;

		clearMap(map);
	}

	template <typename T>
	void clearVector(std::vector<T>& vector)
	{
		std::vector<T> empty;
		vector.swap(empty);
	}

	template <typename T>
	void clearPointerVector(std::vector<T>& vector)
	{
		for(auto it = vector.begin(); it != vector.end(); it++)
			delete *it;

		clearVector(vector);
	}
}