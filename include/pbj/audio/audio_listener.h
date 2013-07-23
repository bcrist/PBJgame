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

#ifndef PBJ_AUDIO_AUDIO_LISTENER
#define PBJ_AUDIO_AUDIO_LISTENER

#include "pbj\audio\audio_source.h"

namespace pbj{
namespace audio{

class AudioListener
{
public:
	AudioListener();
	~AudioListener();

	void setListenerPos(vec3 listenerPos);
	vec3 getListenerPos();

	void setListenerVel(vec3 listenerVel);
	vec3 getListenerVel();

	void setListenerDir(vec3 listenerDir[2]);
	vec3 getListenerAtDir();
	vec3 getListenerUpDir();

private:
	vec3 listenerPos_;
	vec3 listenerVel_;
	vec3 listenerDir_[2];

};

} // namespace audio
} // namespace pbj

#endif
