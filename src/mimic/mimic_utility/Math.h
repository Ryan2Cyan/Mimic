#pragma once

namespace MimicUtility
{
	/// <summary>
	/// Converts inputted value from range [oldMin : oldMax] to new range
	/// [newMin : newMax], and returns the result.
	/// </summary>
	/// <typeparam name="T">Data type of which the calculation will be conducted.</typeparam>
	/// <param name="arg">Value to be converted.</param>
	/// <param name="oldMin">Original minimum range value of 'arg'.</param>
	/// <param name="oldMax">Original maximum range value of 'arg'.</param>
	/// <param name="newMin">New minimum range value of the return result.</param>
	/// <param name="newMax">New maximum range value of the return result.</param>
	/// <returns>Converted value (converted to the new range).</returns>
	template<typename T> static T ConvertToRange(const T& arg, const T& oldMin, const T& oldMax, const T& newMin, const T& newMax)
	{
		return ((arg - oldMin) / oldMax - oldMin) * (newMax - newMin) + newMin;
	}
}