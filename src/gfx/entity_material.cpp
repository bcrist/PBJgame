///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/entity_material.cpp
/// \author Peter Bartosch
/// \date	2013-08-01
/// \brief	Implementation for the \c EntityMaterial class.
#ifndef GFX_ENTITY_MATERIAL_H_
#include "pbj/gfx/gfx_entity_material.h"
#endif

using namespace pbj;
using namespace gfx;

////////////////////////////////////////////////////////////////////////////////
EntityMaterial::EntityMaterial()
{
	_initialized = false;
	_shader = 0;
	_texture = 0;
	_nextCallbackId = 0;
}

////////////////////////////////////////////////////////////////////////////////
EntityMaterial::~EntityMaterial()
{
	if(_initialized)
		destroy();
}

////////////////////////////////////////////////////////////////////////////////
/// \param shader[in] The \sa SceneShaderProgram pointer for the EntityMaterial.
/// \param texture[in] The \sa Texture pointer for the EntityMaterial.
void EntityMaterial::init(SceneShaderProgram* shader, Texture* texture)
{
	assert(!_initialized);
	_shader = shader;
	_texture = texture;

	generateUniformConfigs();
	generateSamplerConfigs();

	_callbacks = vector<pair<U32,ComponentCallback>>();

	_initialized = true;
}

////////////////////////////////////////////////////////////////////////////////
void EntityMaterial::destroy()
{
	if(_initialized)
	{
		//these may be used elsewhere, so don't delete.  just remove the ptr
		_shader = 0;
		_texture = 0;

		//think I should have something here that removes the EntityMaterial from
		//whatever it's attached to as well.  Though these things aren't being
		//destroyed enough to really warrant it.  Should only need to be
		//destroyed during engine teardown.

		_callbacks.clear();
		_nextCallbackId = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////
const SceneShaderProgram* EntityMaterial::getShaderProgram()
{
	return _shader;
}

////////////////////////////////////////////////////////////////////////////////
/// \param newProgram[in] The new \c SceneShaderProgram for this object.
void EntityMaterial::setShaderProgram(SceneShaderProgram* newProgram)
{
	_shader = newProgram;
	generateUniformConfigs();
	doCalls();
}

////////////////////////////////////////////////////////////////////////////////
const Texture* EntityMaterial::getTexture()
{
	return _texture;
}

////////////////////////////////////////////////////////////////////////////////
/// \param newTexture[in] The new \c Texture for this object.
void EntityMaterial::setTexture(Texture* newTexture)
{
	_texture = newTexture;
	generateSamplerConfigs();
	doCalls();
}

////////////////////////////////////////////////////////////////////////////////
void EntityMaterial::generateUniformConfigs()
{
	I32 unused;
	memcpy(_uniforms, _shader->getUniformConfigs(unused), nUniforms);
	doCalls();
}

////////////////////////////////////////////////////////////////////////////////
/// \details This will only work with one texture as configured.  I think that
///          changing the naming convention for texsamplers and some
///          concatenation would make it work with multiple textures.  We do not
///          need to do this now.
void EntityMaterial::generateSamplerConfigs()
{
	for(int i=0;i<nSamplers;++i)
	{
		_samplers[i].texture_id = _texture->getGlId();
		_samplers[i].uniform_location = glGetUniformLocation(_shader->getGlId(), "texsampler");
	}
	doCalls();
}

////////////////////////////////////////////////////////////////////////////////
const UniformConfig* EntityMaterial::getUniformConfigs()
{
	return _uniforms;
}

////////////////////////////////////////////////////////////////////////////////
const SamplerConfig* EntityMaterial::getSamplerConfigs()
{
	return _samplers;
}

////////////////////////////////////////////////////////////////////////////////
/// \param callback[in] The function to call when \sa doCalls() is callled.
/// \returns The id for the passed callback function;
U32 EntityMaterial::addCallback(ComponentCallback callback)
{
	for(vector<pair<U32,ComponentCallback>>::iterator it=_callbacks.begin();
		it!=_callbacks.end();
		++it)
	{
		//if the target pointers are the same, then the callback is already
		//registered and has an id.
		if(it->second.target == callback.target)
			return it->first;
	}

	U32 id = ++_nextCallbackId;
	_callbacks.push_back(pair<U32,ComponentCallback>(id, callback));
	return id;
}

////////////////////////////////////////////////////////////////////////////////
/// \param id[in] The id for the callback function to be removed from the list
///               of callback functions.
void EntityMaterial::removeCallback(U32 id)
{
	vector<pair<U32,ComponentCallback>>& cb = _callbacks;
	for(vector<pair<U32,ComponentCallback>>::iterator it=_callbacks.begin();
		it!=_callbacks.end();
		++it)
	{
		if(it->first == id)
		{
			cb.erase(it);
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
void EntityMaterial::doCalls()
{
	for(vector<pair<U32,ComponentCallback>>::iterator it=_callbacks.begin();
		it!=_callbacks.end();
		++it)
		it->second();
}