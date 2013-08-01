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
/// \brief	Implementation of the Entity class

#ifndef ENTITY_H_
#include "pbj/scene/entity.h"
#endif

using namespace pbj;
using namespace pbj::scene;

////////////////////////////////////////////////////////////////////////////////
Entity::Entity()
{
	_initialized = false;
}

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
void Entity::init()
{
	_batcherTask.n_indices = 0;
	_transform = Transform();
	_transformCallbackId = U32(-1);
	_materialCallbackId = U32(-1);
	_mesh = 0;
	_material = 0;
	_initialized = true;
}

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
/// \brief Returns a pointer to the \sa BatcherTask for this Entity.
/// \return A pointer to the Entity's \sa BatcherTask.
const BatcherTask* Entity::getBatcherTask()
{
	if(_batcherTask.n_indices != 0 && _initialized)
		return &_batcherTask;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief Creates the \sa BatcherTask for the Entity.
/// \details The BatcherTask needs to be recreated everytime the entity changes
///          something that would affect its drawing (position, shape, or 
///          appearance).  If these do not change, then Entity does not need to
///          recreate its BatcherTask and this function does not need to be
///          called.
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

    gfx::UniformConfig* unis;
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
const Transform* Entity::getTransform()
{
	if(_initialized)
		return &_transform;
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
void Entity::setTransform(Transform transform)
{
	if(!_initialized)
		init();
	_transform = transform;
}
	
////////////////////////////////////////////////////////////////////////////////
Mesh* Entity::getMesh()
{
	if(_initialized)
		return _mesh;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
void Entity::setMesh(Mesh* mesh)
{
	if(!_initialized)
		init();
	_mesh = mesh;
}
	
////////////////////////////////////////////////////////////////////////////////
const EntityMaterial* Entity::getMaterial()
{
	if(_initialized)
		return _material;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
void Entity::setMaterial(EntityMaterial* material)
{
	if(!_initialized)
		init();

	if(_material != 0)
		_material->removeCallback(_materialCallbackId);
	_material = material;
	_materialCallbackId = _material->addCallback(ComponentCallback(this->generateBatcherTask));
}