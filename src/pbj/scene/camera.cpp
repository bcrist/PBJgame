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
