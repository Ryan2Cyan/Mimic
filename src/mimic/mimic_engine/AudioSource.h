#pragma once
#include "Resource.h"
#include "Component.h"

namespace MimicAudio
{
	struct AudioSource;
	struct AudioClip;
}

namespace MimicEngine
{
	/// <summary>
	/// Wrapper for MimicAudio::AudioClip. Interfaces with the ResourceManager to be a loadable resource. Once instantiated,
	/// pass this struct into the AudioSource struct. See AudioSource struct for audio functionality.
	/// </summary>
	struct AudioClip : Resource
	{
		bool Load(const std::string& path) override;

	private:
		friend struct AudioSource;
		std::shared_ptr<MimicAudio::AudioClip> _audioAudioClip;
	};

	/// <summary>
	/// Wrapper for MimicAudio::AudioSource. Primary interface for audio functionality. Manipulates an inputted AudioClip (including: Playing 
	/// and Stopping the audio contained within the AudioClip).
	/// </summary>
	struct AudioSource : Component
	{
		/// <summary>
		/// Play sound effect assigned to this source. If null no sound effect will play. To set sound effect see AudioSource::SetAudioClip().
		/// </summary>
		void PlaySfx() const;

		/// <summary>
		/// Stop sound effect assigned to this source. If null nothing will happen. To set sound effect see AudioSource::SetAudioClip().
		/// </summary>
		void StopSfx() const;

		void SetAudioClip(const std::shared_ptr<AudioClip>& audioClip);

		void Initialise() override;
		void Start() override;
		void Update() override;
		void FixedUpdate() override;

	private:
		std::shared_ptr<MimicAudio::AudioSource> _audioSource;
		std::shared_ptr<AudioClip> _audioClip;
	};
}