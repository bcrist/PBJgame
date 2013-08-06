///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/transform.h
/// \author Peter Bartosch
/// \date   2013-07-18
/// \brief  A class for representing the transformation of an object.
/// \details This class is mostly a wrapper for glm calls.  It stores the
///         position, rotation, and scale of an object.  GLM_SWIZZLE is used to
///         convert from the position stored as a vec4 to the position as a
///         vec3.

#ifndef PBJ_TRANSFORM_H_
#define PBJ_TRANSFORM_H_

#ifndef GLM_SWIZZLE
	#define GLM_SWIZZLE
#endif

#include "pbj/_pbj.h"
#include "pbj/_math.h"

namespace pbj {

class Transform
{
public:
	Transform();
	~Transform();

	void rotate(F32, const vec3&);

	void move(F32, F32, F32);
	void move(const vec3&);

	const vec3& getPosition() const;
	void setPosition(F32, F32, F32);
	void setPosition(const vec3&);

	const vec4& getAngleAxis() const;
	void setAngleAxis(F32, F32, F32, F32);
	void setAngleAxis(F32, const vec3&);

	const quat& getRotation() const;

	const vec3& getScale() const;
	void setScale(F32, F32, F32);
	void setScale(const vec3&);

	mat4 getMatrix() const;

	//Entity* getOwner();
private:
	//Entity* _owner
	vec4 _position;	///< The position.
	quat _rotation;	///< Rotation stored as a quaternion.
	vec3 _scale;	///< Scale.
};

} //namespace pbj

#endif
