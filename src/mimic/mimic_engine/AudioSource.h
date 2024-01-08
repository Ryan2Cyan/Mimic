#pragma once
#include "Resource.h"
#include <mimic_audio/AudioClip.h>

namespace MimicEngine
{
	// #############################################################################
	// Audio Clip Struct:
	// #############################################################################
	struct AudioClip : Resource
	{
		bool Load(const std::string& path) override;

	private:
		std::shared_ptr<MimicAudio::AudioClip> _audioAudioClip;
	};
}