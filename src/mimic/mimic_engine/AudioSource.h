#pragma once
#include "Resource.h"
#include "Component.h"
#include <mimic_audio/AudioClip.h>
#include <mimic_audio/AudioSource.h>

namespace MimicEngine
{
	// #############################################################################
	// AudioClip Struct:
	// #############################################################################
	struct AudioClip : Resource
	{
		bool Load(const std::string& path) override;

	private:
		friend struct AudioSource;

		std::shared_ptr<MimicAudio::AudioClip> _audioAudioClip;
	};

	// #############################################################################
	// AudioSource Struct:
	// #############################################################################
	struct AudioSource : Component
	{
		void PlaySfx() const;
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