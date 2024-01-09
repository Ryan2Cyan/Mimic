#include "AudioSource.h"
#include "AudioClip.h"
#include <mimic_utility/Logger.h>

#include <AL/al.h>

namespace MimicAudio
{
	std::shared_ptr<AudioSource> AudioSource::Initialise()
	{
		auto audioSource = std::make_shared<AudioSource>();
		alGenSources(1, &audioSource->_id);
		return audioSource;
	}

	std::shared_ptr<AudioSource> AudioSource::Initialise(const std::shared_ptr<AudioClip>& audioClip)
	{
		auto audioSource = std::make_shared<AudioSource>();
		alGenSources(1, &audioSource->_id);
		audioSource->SetAudioClip(audioClip);
		return audioSource;
	}

	void AudioSource::PlaySfx() const
	{
		if (_id == 0) return;
		if (_audioClip->GetId() == 0) return;
		alSourcePlay(_id);
	}

	void AudioSource::StopSfx() const
	{
		if (_id == 0) return;
		if (_audioClip->GetId() == 0) return;
		alSourceStop(_id);
	}

	void AudioSource::SetAudioClip(const std::shared_ptr<AudioClip>& audioClip)
	{
		if (!audioClip)
		{
			MIMIC_LOG_WARNING("[MimicAudio::MimicSource] Unable to set invalid audio clip.");
			return;
		}
		_audioClip = audioClip;
		alSourcei(_id, AL_BUFFER, _audioClip->GetId());
	}

	unsigned int AudioSource::GetId() const
	{
		return _id;
	}
}