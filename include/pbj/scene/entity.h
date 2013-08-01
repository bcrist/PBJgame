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
/// \file	pbj/entity.h
/// \author	Peter Barotsch
/// \brief	Definition for the Entity class.
/// \details	The entity class is used to keep track of all data relating to
///             something in the game.  It must have a position but any other
///				components on it are optional.

#ifndef ENTITY_H_
#define ENTITY_H_

#include "pbj/_pbj.h"
#include "pbj/transform.h"
#include "pbj/gfx/mesh.h"
#include "pbj/gfx/material.h"
#include "pbj/gfx/batcher.h"

using pbj::gfx::Mesh;
using pbj::gfx::Material;
using pbj::gfx::BatcherTask;

namespace pbj
{
namespace scene
{
	class Entity
	{
	public:
		Entity();
		~Entity();
		
		//intialization and destruction funcs
		void init();
		void destroy();
		
		//BatcherTask related funcs, ignoring const correctness
		const BatcherTask* getBatcherTask();
		void generateBatcherTask();
		
		//accessors, these will expand as the class gains more component
		//possiblities
		Transform* getTransform();
		void setTransform(Transform*);
		
		Mesh* getMesh();
		void setMesh(Mesh*);
		
		Material* getMaterial();
		void setMaterial(Material*);
		
		bool isDepthTested() const;
		void makeDepthTested();
		void makeNotDepthTested();
		
	private:
		bool _initialized;
		BatcherTask _batcherTask;
		
		Transform* _transform;
		Mesh* _mesh;
		Material* _material;
		bool _depthTested
	};
} //namespace pbj::scene
} //namespace pbj
#endif