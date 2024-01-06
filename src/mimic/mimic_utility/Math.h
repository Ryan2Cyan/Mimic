#pragma once

namespace MimicUtility
{
	/// <summary> Converts inputted value from range [oldMin : oldMax] to new range
	/// [newMin : newMax], and returns the result. </summary>
	template<typename T> static T ConvertToRange(const T& arg, const T& oldMin, const T& oldMax, const T& newMin, const T& newMax)
	{
		return ((arg - oldMin) / oldMax - oldMin) * (newMax - newMin) + newMin;
	}
}