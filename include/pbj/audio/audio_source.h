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
/// \file   pbj/audio/audio_source.h
/// \author Josh Douglas
///
/// \brief  pbj::audio::AudioBuffer class header.

#ifndef PBJ_AUDIO_AUDIO_SOURCE
#define PBJ_AUDIO_AUDIO_SOURCE

#include "audio_buffer.h"
#include "pbj\_math.h"
#include <vector>

namespace pbj{
namespace audio{

///////////////////////////////////////////////////////////////////////////////
/// \brief  Audio source header.
class AudioSource
{
public:
	AudioSource();
	~AudioSource();

	void setSourcePos(vec3 sourcePos);
	vec3 getSourcePos();

	void setSourceVel(vec3 sourceVel);
	vec3 getSourceVel();

	void genSources(ALuint numSources, ALuint &sourceID);
	void bindBuffer(const AudioBuffer &buffer);

	void loop(ALuint &sourceID);

	void play(ALuint &sourceID);
	void stop(ALuint &sourceID);
	void pause(ALuint &sourceID);

private:
	vec3 sourcePos_;
	vec3 sourceVel_;

	ALuint sourceID_;

};

} // namespace audio
} // namespace pbj

#endif
