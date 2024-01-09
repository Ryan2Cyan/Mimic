#pragma once
#include <memory>

namespace MimicAudio
{
	// #############################################################################
	// AudioSource Struct:
	// #############################################################################

	struct AudioClip;
	struct AudioSource
	{
		static std::shared_ptr<AudioSource> Initialise();
		static std::shared_ptr<AudioSource> Initialise(const std::shared_ptr<AudioClip>& audioClip);

		void PlaySfx() const;
		void StopSfx() const;
		void SetAudioClip(const std::shared_ptr<AudioClip>& audioClip);
		unsigned int GetId() const;

	private:
		std::shared_ptr<AudioClip> _audioClip;
		unsigned int _id;
	};
}