#pragma once
#include <memory>

namespace MimicAudio
{
	struct AudioClip;

	/// <summary>
	/// Primary interface for audio functionality. Manipulates an inputted AudioClip (including: Playing 
	/// and Stopping the audio contained within the AudioClip).
	/// </summary>
	struct AudioSource
	{
		static std::shared_ptr<AudioSource> Initialise();
		static std::shared_ptr<AudioSource> Initialise(const std::shared_ptr<AudioClip>& audioClip);

		/// <summary>
		/// Play sound effect assigned to this source. If null no sound effect will play. To set sound effect see AudioSource::SetAudioClip().
		/// </summary>
		void PlaySfx() const;

		/// <summary>
		/// Stop sound effect assigned to this source. If null nothing will happen. To set sound effect see AudioSource::SetAudioClip().
		/// </summary>
		void StopSfx() const;

		void SetAudioClip(const std::shared_ptr<AudioClip>& audioClip);
		unsigned int GetId() const;

	private:
		std::shared_ptr<AudioClip> _audioClip;
		unsigned int _id;
	};
}