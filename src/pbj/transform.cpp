///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/transform.cpp
/// \author Peter Bartosch
/// \date   2013-07-18
/// \brief  Implementation for Transform class

#include "pbj/transform.h"

namespace pbj {

///////////////////////////////////////////////////////////////////////////////
/// \brief Constructor for Transform class
/// \details Sets the position, rotation and scale for the Transform to
///          defaults.  For position this is the origin, rotation the
///          identity quaternion, and for scale 1.0.
Transform::Transform()
{
	_position = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	_rotation = quat();
	_scale = vec3(1.0f);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Destructor for Transform class, left empty.
Transform::~Transform()
{}

///////////////////////////////////////////////////////////////////////////////
/// \brief Rotate the Transform along a certain axis.
/// \param angle The angle to rotate, in degrees.
/// \param axis The axis along which the rotation occurs.
void Transform::rotate(F32 angle, const vec3& axis)
{
	_rotation = glm::rotate(_rotation, angle, axis);
	/*
	if(_owner->rigidbody!=0)
		//convert y rotation to euler angles and set the rigidbody accordingly
	*/
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Move the Transform.
/// \param dx The distance to move along the x-axis.
/// \param dy The distance to move along the y-axis.
/// \param dz The distance to move along the z-axis.
void Transform::move(F32 dx, F32 dy, F32 dz)
{
	vec3 v = _position.xyz;
	mat4 m = glm::translate(mat4(), v);
	_position = m * vec4(dx, dy, dz, 1.0f);
	/*
	if(_owner->rigidbody!=0)
		_owner->rigidbody->moveBody(_position.x, _position.y);
	*/
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Move the transform
/// \param deltas A glm::vec3 containing the distance to move along each axis.
void Transform::move(const vec3& deltas)
{
	move(deltas.x, deltas.y, deltas.z);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief The position of the Transform.
/// \return The position of the Transform as a glm::vec3 (x,y,z).
const vec3& Transform::getPosition() const
{
	return _position.xyz;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Sets the position of the Transform
/// \param x The x position
/// \param y The y position
/// \param z The z position
void Transform::setPosition(F32 x, F32 y, F32 z)
{
	_position = vec4(x, y, z, 1.0f);
	/*
	if(_owner->rigidbody!=0)
		_owner->rigidbody->moveBody(_position.x, _position.y);
	*/
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Sets the position of the Transform
/// \param pos The new position as a glm::vec3.
void Transform::setPosition(const vec3& pos)
{
	setPosition(pos.x, pos.y, pos.z);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Get the rotation of the Transform.
/// \return A glm::vec4 containing the angle of rotation (in degrees) and the
///         x, y, and z coordinates of the vector that forms the axis of
///         rotation.
/// \details Keep in mind that the returned angles are not necessarily
///          equivilent to Euler angles and that further conversion is
///          necessary to get those angle values.
const vec4& Transform::getAngleAxis() const
{
	F32 angle = glm::angle(_rotation);
	vec3 axis = glm::axis(_rotation);
	return vec4(angle, axis.x, axis.y, axis.z);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Set the rotation of the Transform.
/// \param angle The angle of rotation.
/// \param x The x value of the axis of rotation.
/// \param y The y value of the axis of rotation.
/// \param z The z value of the axis of rotation.
void Transform::setAngleAxis(F32 angle, F32 x, F32 y, F32 z)
{
	_rotation = glm::angleAxis(angle, x, y, z);
	/*
	if(_owner->rigidbody!=0)
		//convert y rotation to euler angles and set the rigidbody accordingly
	*/
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Set the rotation of the Transform.
/// \param angle The angle of rotation.
/// \param axis The axis of rotation.
void Transform::setAngleAxis(F32 angle, const glm::vec3& axis)
{
	_rotation = glm::angleAxis(angle, glm::normalize(axis));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Get the rotation of the Transform as a quaternion
/// \return A quaternion representing the rotation of the Transform.
/// \details I have no idea why we might want to get the rotation is this manner
///          but it seems like a good idea.  So here it is.
const quat& Transform::getRotation() const
{
	return _rotation;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Get the scale of the Transform.
/// \return A glm::vec3 representing the scale along the x-, y-, and z- axis.
const vec3& Transform::getScale() const
{
	return _scale;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Set the scale of the Transform
/// \param x The scale along the x-axis.
/// \param y The scale along the y-axis.
/// \param z The scale along the z-axis.
void Transform::setScale(F32 x, F32 y, F32 z)
{
	_scale = glm::vec3(x, y, z);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Set the scale of the Transform
/// \param scale A glm::vec3 of for the scale in the x, y, and z directions.
void Transform::setScale(const vec3& scale)
{
	_scale = scale;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Get the Transform as a 4x4 matrix.
/// \returns The Transform as a 4x4 matrix.
/// \details Since our call require a transformation matrix, this will provide
///          one.
mat4 Transform::getMatrix() const
{
	mat4 ret = mat4();
	vec3 v = _position.xyz;
	ret = glm::translate(ret, v);
	ret = glm::rotate(ret, glm::angle(_rotation), glm::axis(_rotation));
	ret = glm::scale(ret, _scale);
	return ret;
}

/*
Entity* Transform::getOwner()
{
	return _owner;
}
*/
} // namespace pbj
