#include "AudioClip.h"
#include <mimic_utility/Logger.h>

#include <vector>
#include <AL/al.h>
#include <AL/alc.h>
#include "stb_vorbis.c"

namespace MimicAudio
{
	static bool load_ogg(const std::string& path, std::vector<unsigned char>& buffer, ALenum& format, ALsizei& freq)
	{
		int channels = 0;
		int sampleRate = 0;
		short* output = NULL;

		size_t samples = stb_vorbis_decode_filename(path.c_str(), &channels, &sampleRate, &output);

		if (samples == -1)
		{
			MIMIC_LOG_WARNING("[MimicAudio::AudioSource] Unable to load audio source from path: \"%\"", path);
			return false;
		}

		// Record the format required by OpenAL.
		if (channels < 2) format = AL_FORMAT_MONO16;
		else format = AL_FORMAT_STEREO16;

		// Copy (# samples) * (1 or 2 channels) * (16 bits == 2 bytes == short).
		buffer.resize(samples * channels * sizeof(short));
		memcpy(&buffer.at(0), output, buffer.size());

		// Record the sample rate required by OpenAL.
		freq = sampleRate;

		// Clean up the read data.
		free(output);

		return true;
	}

	std::shared_ptr<AudioClip> AudioClip::Initialise(const std::string& path)
	{
		// Load .ogg audio file.
		int format = 0;
		int freq = 0;
		std::vector<unsigned char> bufferData;
		if (!load_ogg(path, bufferData, format, freq)) return nullptr;

		unsigned int bufferId = 0;
		alGenBuffers(1, &bufferId);

		alBufferData(bufferId, format, &bufferData.at(0), static_cast<size_t>(bufferData.size()), freq);

		
		// Prepare sound source:
		unsigned int sourceId = 0;
		alGenSources(1, &sourceId);
		alSourcei(sourceId, AL_BUFFER, bufferId);

		auto audioClip = std::make_shared<AudioClip>();
		audioClip->_bufferId = bufferId;
		return audioClip;
	}

	unsigned int AudioClip::GetId() const
	{
		return _bufferId;
	}
}