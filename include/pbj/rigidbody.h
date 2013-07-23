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
/// \file   pbj/rigidbody.h
/// \author Peter Bartosch
/// \date   2013-07-23
/// \brief  A class for representing an Entity's rigidbody for collisions and
///         physics.
/// \details This is a wrapper for Box2D's b2Body type.  I did not implement
///          everything from Box2D as we do not need everything.  Since we have
///          no collision detection for objects that are not rigidbodies, I felt
///          that it would be easier (if less flexible) to not have a Collider
///          class.  Instead, all colliders and associated shapes are dealt with
///          in this class.  Since the Entity class is not yet written, any
///          functionality regarding that is commented out.
#ifndef RIGIDBODY_H_
#define RIGIDBODY_H_

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include "_math.h"
#include "_pbj.h"

namespace pbj
{
class Rigidbody
{
public:
	enum ForceMode
	{
		Constant = 0x01,
		Impulse = 0x02
	};

	enum BodyType
	{
		Static = 0x00,
		Dynamic = 0x02
	};

	//Rigidbody(Rigidbody::BodyType, const b2Shape*, b2World*, Entity&);
	Rigidbody(Rigidbody::BodyType, const b2Shape*, b2World*);
	virtual ~Rigidbody()=0;

	b2Fixture* getFixtureList();
	const b2Fixture* getFixtureList() const;
	void destroyFixture(b2Fixture*);

	b2ContactEdge* getContactList();
	const b2ContactEdge* getContactList() const;

	void applyForce(const vec2&); ///< Applies to center of mass.  Assumes ForceMode::Constant
	void applyForce(const vec2&, Rigidbody::ForceMode);

	vec2 getVelocity() const;
	void setVelocity(const vec2&);
	
	F32 getMass();
	void setMass(F32);

	Rigidbody::BodyType getType() const;

	void setBullet(bool); ///< To turn on CCD with other dynamic bodies.
	bool isBullet();

	bool isAwake();
	bool isActive();

	int getCollisionGroup();
	void setCollisionGroup(int); ///< negative for no collision with group members
	
	//Entity* getOwner();
	//void moveOwner();

	void destroy();
private:
	b2Body* _body;
	//Entity* _owner;
};
} //namespace pbj
#endif