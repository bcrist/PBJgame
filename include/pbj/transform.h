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

	vec3 getPosition() const;
	void setPosition(F32, F32, F32);
	void setPosition(const vec3&);

	vec4 getAngleAxis() const;
	void setAngleAxis(F32, F32, F32, F32);
	void setAngleAxis(F32, const vec3&);

	quat getRotation() const;

	vec3 getScale() const;
	void setScale(F32, F32, F32);
	void setScale(const vec3&);

	mat4 getMatrix() const;
private:
	vec4 _position;	///< The position.
	quat _rotation;	///< Rotation stored as a quaternion.
	vec3 _scale;	///< Scale.
};

} //namespace pbj

#endif
