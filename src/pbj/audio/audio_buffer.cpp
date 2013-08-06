///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/audio/audio_buffer.cpp
/// \author Josh Douglas
///
/// \brief  pbj::audio::AudioBuffer class source.

#include "pbj/audio/audio_buffer.h"

namespace pbj {
namespace audio {

AudioBuffer::AudioBuffer(const ALubyte* data, size_t size)
{
    buffer_id_ = alutCreateBufferFromFileImage(data, size);
}

AudioBuffer::~AudioBuffer()
{
}

ALuint AudioBuffer::getBufferID()
{
	return buffer_id_;
}

} // namespace pbj::audio
} // namespace pbj
