#pragma once
#include <memory>
#include <string>

namespace MimicAudio 
{
	// #############################################################################
	// AudioSource Struct:
	// #############################################################################
	struct AudioClip
	{
		static std::shared_ptr<AudioClip> Initialise(const std::string& path);

		unsigned int GetId() const;

	private:
		unsigned int _bufferId;
	};
}