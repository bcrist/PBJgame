///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/entity_material.h
/// \author Peter Bartosch
/// \date	2013-08-01
/// \brief	This class puts \sa Texture and \sa SceneShaderProgram in one object
///         for easier management at the \sa Entity level.
/// \details	The idea is that an \c Entity has three portions relevant to
///             drawing.  The first is its transformation, which is handled by
///             \sa Transform.  The second is geometry, which is handled by
///             \sa Mesh for the time being.  The third is appearance, which is
///             what this class is designed to manage.  It should be noted that
///             this class is currently made to only work with one texture.
#ifndef GFX_ENTITY_MATERIAL_H_
#define GFX_ENTITY_MATERIAL_H_

#include <iostream>
#include <assert.h>
#include <vector>
#include <functional>

#include "pbj/_pbj.h"
#include "pbj/gfx/batcher.h"
#include "pbj/gfx/gfx_scene_shader_program.h"
#include "pbj/gfx/texture.h"

using std::vector;
using std::pair;
using std::function;

namespace pbj
{
namespace gfx
{
	typedef std::function<void()> ComponentCallback;

	class EntityMaterial
	{
	public:
		static const unsigned int nUniforms = 12;
		static const unsigned int nSamplers = 1;

		EntityMaterial();
		~EntityMaterial();

		void init(SceneShaderProgram*, Texture*);
		void destroy();
		
		const SceneShaderProgram* getShaderProgram();
		void setShaderProgram(SceneShaderProgram*);

		const Texture* getTexture();
		void setTexture(Texture*);

		void generateUniformConfigs();
		void generateSamplerConfigs();

		const UniformConfig* getUniformConfigs();
		const SamplerConfig* getSamplerConfigs();

		U32 addCallback(ComponentCallback);
		void removeCallback(U32);

	private:
		void doCalls();

		bool _initialized;
		U32 _nextCallbackId;
		SceneShaderProgram* _shader;
		Texture* _texture;

		UniformConfig _uniforms[nUniforms];
		SamplerConfig _samplers[nSamplers];

		vector<pair<U32,ComponentCallback>> _callbacks;
	};
} //namespace pbj
} //namespace pbj::gfx
#endif