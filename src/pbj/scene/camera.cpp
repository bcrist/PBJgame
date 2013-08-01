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
/// \file   pbj/scene/camera.cpp
/// \author Ben Crist

#include "pbj/scene/camera.h"

namespace pbj {
namespace scene {

///////////////////////////////////////////////////////////////////////////////
Camera::Camera()
{
}

///////////////////////////////////////////////////////////////////////////////
Camera::~Camera()
{
}

///////////////////////////////////////////////////////////////////////////////
void Camera::setProjection(const mat4& projection)
{
    projection_ = projection;
}

///////////////////////////////////////////////////////////////////////////////
void Camera::setTargetPosition(const vec3& position)
{
    target_position_ = position_;
}

///////////////////////////////////////////////////////////////////////////////
void Camera::setTargetVelocity(const vec3& velocity)
{
    target_velocity_ = velocity_;
}

///////////////////////////////////////////////////////////////////////////////
const mat4& Camera::getProjection() const
{
    return projection_;
}

///////////////////////////////////////////////////////////////////////////////
const mat4& Camera::getView() const
{
    return view_;
}

///////////////////////////////////////////////////////////////////////////////
void Camera::update(double delta_t)
{
    const F32 pos_coeff = 1.9f;
    const F32 vel_coeff = 1.5f;

    vec3 acceleration = (target_position_ - position_) * pos_coeff;
    acceleration *= (target_velocity_ - velocity_) * vel_coeff;

    position_ += velocity_ * float(delta_t) +
                 acceleration * float(delta_t * delta_t * 0.5);

    velocity_ += acceleration * float(delta_t);

    view_ = glm::translate(mat4(), -position_);
}

} // namespace pbj::scene
} // namespace pbj
