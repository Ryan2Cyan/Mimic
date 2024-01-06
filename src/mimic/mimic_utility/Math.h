#pragma once

namespace MimicUtility
{
	template<typename T>
	static T ConvertToRange(const T& arg, const T& oldMin, const T& oldMax, const T& newMin, const T& newMax)
	{
		return ((arg - oldMin) / oldMax - oldMin) * (newMax - newMin) + newMin;
	}
}