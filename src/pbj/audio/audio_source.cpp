// Copyright (c) 2013 PBJ^2 Productions
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/audio/audio_source.cpp
/// \author Josh Douglas
///
/// \brief  pbj::audio::AudioBuffer class source.

#include "pbj\audio\audio_source.h"

namespace pbj{
namespace audio{

AudioSource::AudioSource()
{
	alGenSources(1, &sourceID_);
}

AudioSource::~AudioSource()
{
	if(alIsSource(sourceID_))
	{
		alDeleteSources(1, &sourceID_);
	}
}

void AudioSource::setListenerPos(vec3 listenerPos)
{
	listenerPos_ = listenerPos;
	alListenerfv(AL_POSITION, glm::value_ptr(listenerPos_));
}

vec3 AudioSource::getListenerPos()
{
	return listenerPos_;
}

void AudioSource::setListenerVel(vec3 listenerVel)
{
	listenerVel_ = listenerVel;
	alListenerfv(AL_VELOCITY, glm::value_ptr(listenerVel_));
}

vec3 AudioSource::getListenerVel()
{
	return listenerVel_;
}

void AudioSource::setListenerDir(vec3 listenerDir[2])
{
	listenerDir_[0] = listenerDir[0];
	listenerDir_[1] = listenerDir[1];
	alListenerfv(AL_ORIENTATION, glm::value_ptr(listenerDir_[0]));
	alListenerfv(AL_ORIENTATION, glm::value_ptr(listenerDir_[1]));
}

vec3 AudioSource::getListenerAtDir()
{
	return listenerDir_[0];
}

vec3 AudioSource::getListenerUpDir()
{
	return listenerDir_[1];
}

void AudioSource::setSourcePos(vec3 sourcePos)
{
	sourcePos_ = sourcePos;
	alSourcefv(sourceID_, AL_POSITION, glm::value_ptr(sourcePos_));
}

vec3 AudioSource::getSourcePos()
{
	return sourcePos_;
}

void AudioSource::setSourceVel(vec3 sourceVel)
{
   sourceVel_ = sourceVel;
   alSourcefv(sourceID_, AL_VELOCITY, glm::value_ptr(sourceVel_));
}

vec3 AudioSource::getSourceVel()
{
   return sourceVel_;
}

void AudioSource::bindBuffer(const AudioBuffer &buffer)
{
   alSourcei(sourceID_, AL_BUFFER, buffer.getBufferID());
}

void AudioSource::loop(ALuint &sourceID)
{
	alSourcef(sourceID, AL_LOOPING, AL_TRUE);
}

void AudioSource::play(ALuint &sourceID)
{
	alSourcePlay(sourceID);
}

void AudioSource::stop(ALuint &sourceID)
{
	alSourceStop(sourceID);
}

} // namespace audio
} // namespace pbj