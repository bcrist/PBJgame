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
}

AudioSource::~AudioSource()
{
}

void AudioSource::setListenerPos(vec3 listenerPos)
{
	lisenterPos_ = listenerPos;
}

vec3 AudioSource::getListenerPos()
{
	return lisenterPos_;
}

void AudioSource::setListenerVel(vec3 listenerVel)
{
	listenerVel_ = listenerVel;
}

vec3 AudioSource::getListenerVel()
{
	return listenerVel_;
}

void AudioSource::setListenerDir(vec3 listenerDir)
{
	listenerDir_ = listenerDir;
}

vec3 AudioSource::getListenerDir()
{
	return listenerDir_;
}

void AudioSource::setSourcePos(vec3 sourcePos)
{
	sourcePos_ = sourcePos;
}

vec3 AudioSource::getSourcePos()
{
	return sourcePos_;
}

} // namespace audio
} // namespace pbj