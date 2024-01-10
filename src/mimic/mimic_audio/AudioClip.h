#pragma once
#include <memory>
#include <string>

namespace MimicAudio 
{
	/// <summary>
	/// Once instantiated, pass this struct into the AudioSource struct. See AudioSource struct for audio functionality.
	/// </summary>
	struct AudioClip
	{
		static std::shared_ptr<AudioClip> Initialise(const std::string& path);
		unsigned int GetId() const;

	private:
		unsigned int _bufferId;
	};
}