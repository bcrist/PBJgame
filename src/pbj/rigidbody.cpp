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
/// \file   pbj/rigidbody.cpp
/// \author Peter Bartosch
/// \date   2013-07-23

#ifndef RIGIDBODY_H_
#include "pbj/rigidbody.h"
#endif

using namespace pbj;

/*Rigidbody::Rigidbody(Rigidbody::BodyType bodyType, const b2Shape* shape, b2World* physWorld, Entity& owner)
{
	b2BodyDef bd;
	bd.type = (b2BodyType)bodyType;
	bd.position.SetZero();
	bd.angle = 0.0f;
	bd.linearDamping = 0.0f;
	bd.allowSleep = true;
	bd.awake = true;
	bd.bullet = false;
	bd.active = true;
	_body = physWorld->CreateBody(&bd);
	_body->CreateFixture(shape, 1.0f);

	_owner = owner;
	_body->SetUserData(_owner);
}*/

/// \summary Delete this when Entity is implemented
Rigidbody::Rigidbody(Rigidbody::BodyType bodyType, const b2Shape* shape, b2World* physWorld)
{
	b2BodyDef bd;
	bd.type = (b2BodyType)bodyType;
	bd.position.SetZero();
	bd.angle = 0.0f;
	bd.linearDamping = 0.0f;
	bd.allowSleep = true;
	bd.awake = true;
	bd.bullet = false;
	bd.active = true;
	_body = physWorld->CreateBody(&bd);
	_body->CreateFixture(shape, 1.0f);
}

Rigidbody::~Rigidbody()
{
	if(_body != 0)// || _owner !=0)
		destroy();
}


b2Fixture* Rigidbody::getFixtureList()
{
	return _body->GetFixtureList();
}

const b2Fixture* Rigidbody::getFixtureList() const
{
	return _body->GetFixtureList();
}

void Rigidbody::destroyFixture(b2Fixture* fixture)
{
	_body->DestroyFixture(fixture);
}


b2ContactEdge* Rigidbody::getContactList()
{
	return _body->GetContactList();
}

const b2ContactEdge* Rigidbody::getContactList() const
{
	return _body->GetContactList();
}


void Rigidbody::applyForce(const vec2& force)
{
	_body->ApplyForceToCenter(b2Vec2(force.x, force.y));
}

void Rigidbody::applyForce(const vec2& force, Rigidbody::ForceMode mode)
{
	switch(mode)
	{
	case Rigidbody::ForceMode::Constant:
		_body->ApplyForceToCenter(b2Vec2(force.x, force.y));
		break;
	case Rigidbody::ForceMode::Impulse:
		_body->ApplyLinearImpulse(b2Vec2(force.x,force.y), _body->GetPosition());
		break;
	default:
		break;
	}
}


vec2 Rigidbody::getVelocity() const
{
	b2Vec2 v = _body->GetLinearVelocity();
	return vec2(v.x, v.y);
}

void Rigidbody::setVelocity(const vec2& v)
{
	_body->SetLinearVelocity(b2Vec2(v.x, v.y));
}


F32 Rigidbody::getMass()
{
	_body->GetMass();
}

void Rigidbody::setMass(F32 m)
{
	b2MassData md;
	md.mass = m;
	_body->SetMassData(&md);
}


Rigidbody::BodyType Rigidbody::getType() const
{
	return (Rigidbody::BodyType)(_body->GetType());
}


void Rigidbody::setBullet(bool bullet)
{
	_body->SetBullet(bullet);
}

bool Rigidbody::isBullet()
{
	return _body->IsBullet();
}


bool Rigidbody::isAwake()
{
	return _body->IsAwake();
}

bool Rigidbody::isActive()
{
	return _body->IsActive();
}

/*Entity* Rigidbody::getOwner()
{
	return _owner;
}*/

/*void Rigidbody::moveOwner()
{
	vec2 bPos = _body->GetPosition();
	vec3 curPos = vec3(bPos.x, bPos.y, _body->GetUserData()->getPosition().z);
	_body->GetUserData()->setPosition(curPos);
}*/

/// \details Since the rigidbody is being destroyed, I assume the b2body should
///          be destroyed as well.  But we cannot assume the owner Entity needs
///          to be destroyed, so I'll just NULL the pointer.
void Rigidbody::destroy()
{
	//hope this works.  otherwise need a temp b2World*
	_body->GetWorld()->DestroyBody(_body);
	_body = 0;
	//_owner = 0;
}