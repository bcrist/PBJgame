///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/audio/audio_buffer.h
/// \author Josh Douglas
///
/// \brief  pbj::audio::AudioBuffer class header.

#ifndef PBJ_AUDIO_AUDIO_BUFFER_H_
#define PBJ_AUDIO_AUDIO_BUFFER_H_

#include "pbj/_al.h"
#include "pbj/_pbj.h"

namespace pbj {
namespace audio {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Audio element.
class AudioBuffer
{
public:
	AudioBuffer(const ALubyte* data, size_t size);
	~AudioBuffer();
	
	ALuint getBufferID();

private:
	ALuint buffer_id_;
};

} // namespace pbj::audio
} // namespace pbj

#endif
