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
/// \file   pbj/scene/camera.h
/// \author Ben Crist

#ifndef PBJ_SCENE_CAMERA_H_
#define PBJ_SCENE_CAMERA_H_

#include "pbj/_pbj.h"
#include "pbj/_math.h"

namespace pbj {
namespace scene {

class Camera
{
public:
    Camera();
    ~Camera();

    void setProjection(const mat4& projection);
    void setTargetPosition(const vec3& position);
    void setTargetVelocity(const vec3& velocity);

    const mat4& getProjection() const;
    const mat4& getView() const;

    void update(double delta_t);

private:

    vec3 position_;
    vec3 velocity_;
    vec3 target_position_;
    vec3 target_velocity_;

    mat4 projection_;
    mat4 view_;

    Camera(const Camera&);
    void operator=(const Camera&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
