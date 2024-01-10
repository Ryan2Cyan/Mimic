#include "AudioSource.h"
#include <mimic_audio/AudioSource.h>
#include <mimic_audio/AudioClip.h>

namespace MimicEngine
{
	bool AudioClip::Load(const std::string& path)
	{
		_audioAudioClip = MimicAudio::AudioClip::Initialise(path);
		return _audioAudioClip != nullptr;
	}

	void AudioSource::PlaySfx() const
	{
		if (!_audioClip)
		{
			MIMIC_LOG_WARNING("[MimicEngine::AudioSource] Unable to play sound effect, invalid AudioClip.");
			return;
		}
		_audioSource->PlaySfx();
	}

	void AudioSource::StopSfx() const
	{
		if (!_audioClip)
		{
			MIMIC_LOG_WARNING("[MimicEngine::AudioSource] Unable to stop sound effect, invalid AudioClip.");
			return;
		}
		_audioSource->StopSfx();
	}

	void AudioSource::SetAudioClip(const std::shared_ptr<AudioClip>& audioClip)
	{
		_audioClip = audioClip;
		_audioSource->SetAudioClip(audioClip->_audioAudioClip);
	}

	void AudioSource::Initialise()
	{
		_audioSource = MimicAudio::AudioSource::Initialise();
	}

	void AudioSource::Start()
	{

	}

	void AudioSource::Update()
	{

	}

	void AudioSource::FixedUpdate()
	{

	}
}