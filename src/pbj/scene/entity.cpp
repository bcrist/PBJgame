////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJgame\src\pbj\scene\entity.cpp
///
/// \brief	Implements the entity class.
////////////////////////////////////////////////////////////////////////////////
#ifndef ENTITY_H_
#include "pbj/scene/entity.h"
#endif

using namespace pbj;
using namespace pbj::scene;

////////////////////////////////////////////////////////////////////////////////
/// \fn	Entity::Entity()
///
/// \brief	Default constructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
Entity::Entity()
{
	_initialized = false;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Entity::~Entity()
///
/// \brief	Destructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
Entity::~Entity()
{
	if(_initialized)
	{
		destroy();
		_initialized = false;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::init()
///
/// \brief	Initialises this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
void Entity::init()
{
	_batcherTask.n_indices = 0;
	_transform = Transform();
	_transformCallbackId = U32(-1);
	///< An enum constant representing the material callback identifier option
	_materialCallbackId = U32(-1);
	_mesh = 0;
	_material = 0;
	_initialized = true;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::destroy()
///
/// \brief	Destroys this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
void Entity::destroy()
{
	if(_mesh != 0)
	{
		//any deinit should be called before delete
		delete _mesh;
		_mesh = 0;
	}
	
	if(_material != 0)
	{
		//any deinit should be called before delete
		delete _material;
		_material = 0;
	}
	
	//Since there isn't really a decent way to teardown the task, it makes sense
	//to say it has no vertices to render.  This should keep it from getting
	//returned.
	_batcherTask.n_indices = 0;
	
	_initialized = false;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	const BatcherTask* Entity::getBatcherTask()
///
/// \brief	Gets batcher task.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	null if it fails, else the batcher task.
////////////////////////////////////////////////////////////////////////////////
const BatcherTask* Entity::getBatcherTask()
{
	if(_batcherTask.n_indices != 0 && _initialized)
		return &_batcherTask;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::generateBatcherTask()
///
/// \brief	Generates a batcher task.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
/// \details	The BatcherTask needs to be recreated everytime the entity
///				changes something that would affect its drawing (position,
///				shape, or appearance).  If these do not change, then Entity does
///				not need to recreate its BatcherTask and this function does not
///				need to be called.
////////////////////////////////////////////////////////////////////////////////
void Entity::generateBatcherTask()
{
	if(!_initialized)
	{
		init();
		return; //since the default task after init is good, we're done here
	}
	assert(_mesh!=0);
	assert(_material!=0);

	_batcherTask.order_index;
	
    _batcherTask.program_id = _material->getShaderProgram()->getGlId();
	
	//At least geometry is easy.
    _batcherTask.vao_id = _mesh->getVaoId();
    _batcherTask.n_indices = _mesh->getIndexCount();
    _batcherTask.index_data_type = _mesh->getIndexType();
	
    _batcherTask.samplers = _material->getSamplerConfigs();
    _batcherTask.n_samplers = EntityMaterial::nSamplers;

	//still need to figure out data population from here
	_batcherTask.n_uniforms = EntityMaterial::nUniforms;

    gfx::UniformConfig unis[EntityMaterial::nUniforms];

	//want a copy because a material may be shared across Entity objects
	memcpy(unis, _material->getUniformConfigs(), EntityMaterial::nUniforms);

	//the checks and assignments in here are not what I'd call "great".  This is
	//mostly done to have something to work from.
	for(int i=0;i<EntityMaterial::nUniforms;++i)
	{
		if(gfx::SceneShaderProgram::indexToName[i] == "projection_matrix")
		{
			unis[i].data = &_transform.getMatrix();
		}
		else if(gfx::SceneShaderProgram::indexToName[i] == "modelview_matrix")
		{
			unis[i].data = &_transform.getMatrix();
		}
		else if(gfx::SceneShaderProgram::indexToName[i] == "mvp_matrix")
		{
			unis[i].data = &_transform.getMatrix();
		}
	}
	_batcherTask.uniforms = unis;
	
    _batcherTask.depth_tested = false;
	
	//Ignoring for now
    _batcherTask.scissor = 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	const Transform* Entity::getTransform()
///
/// \brief	Gets the transform.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	null if it fails, else the transform.
////////////////////////////////////////////////////////////////////////////////
const Transform* Entity::getTransform()
{
	if(_initialized)
		return &_transform;
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::setTransform(Transform transform)
///
/// \brief	Sets a transform.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	transform	The transform.
////////////////////////////////////////////////////////////////////////////////
void Entity::setTransform(Transform transform)
{
	if(!_initialized)
		init();
	_transform = transform;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Mesh* Entity::getMesh()
///
/// \brief	Gets the mesh.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	null if it fails, else the mesh.
////////////////////////////////////////////////////////////////////////////////
Mesh* Entity::getMesh()
{
	if(_initialized)
		return _mesh;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::setMesh(Mesh* mesh)
///
/// \brief	Sets a mesh.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param [in,out]	mesh	If non-null, the mesh.
////////////////////////////////////////////////////////////////////////////////
void Entity::setMesh(Mesh* mesh)
{
	if(!_initialized)
		init();
	_mesh = mesh;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	const EntityMaterial* Entity::getMaterial()
///
/// \brief	Gets the material.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	null if it fails, else the material.
////////////////////////////////////////////////////////////////////////////////
const EntityMaterial* Entity::getMaterial()
{
	if(_initialized)
		return _material;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::setMaterial(EntityMaterial* material)
///
/// \brief	Sets a material.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param [in,out]	material	If non-null, the material.
////////////////////////////////////////////////////////////////////////////////
void Entity::setMaterial(EntityMaterial* material)
{
	if(!_initialized)
		init();

	if(_material != 0)
		_material->removeCallback(_materialCallbackId);
	_material = material;
	_materialCallbackId = _material->addCallback([&](){ this->generateBatcherTask(); });
}