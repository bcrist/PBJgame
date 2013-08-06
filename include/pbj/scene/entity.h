////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJgame\include\pbj\scene\entity.h
///
/// \brief	Declares the entity class.
////////////////////////////////////////////////////////////////////////////////
#ifndef ENTITY_H_
#define ENTITY_H_

#include "pbj/gfx/mesh.h"
#include "pbj/transform.h"

#include "pbj/_pbj.h"
#include "pbj/transform.h"
#include "pbj/gfx/mesh.h"
#include "pbj/gfx/gfx_entity_material.h"
#include "pbj/gfx/batcher.h"

using pbj::gfx::Mesh;
using pbj::gfx::EntityMaterial;
using pbj::gfx::BatcherTask;
using pbj::gfx::ComponentCallback;

namespace pbj
{
namespace scene
{
	////////////////////////////////////////////////////////////////////////////
	/// \class	Entity
	///
	/// \brief	Entity keeps track of all data relating to an object within a
	/// 		scene.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	////////////////////////////////////////////////////////////////////////////
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
		const Transform* getTransform();
		void setTransform(Transform);
		
		Mesh* getMesh();
		void setMesh(Mesh*);
		
		const EntityMaterial* getMaterial();
		void setMaterial(EntityMaterial*);

	private:
		bool _initialized;
		BatcherTask _batcherTask;
		
		U32 _transformCallbackId;
		U32 _materialCallbackId;

		//components
		Transform _transform;
		Mesh* _mesh;
		EntityMaterial* _material;
	};
} //namespace pbj::scene
} //namespace pbj
#endif
