#include "AudioSource.h"

namespace MimicEngine
{
	bool AudioClip::Load(const std::string& path)
	{
		_audioAudioClip = MimicAudio::AudioClip::Initialise(path);
		return _audioAudioClip != nullptr;
	}
}