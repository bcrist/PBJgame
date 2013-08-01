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
/// \file   pbj/gfx/gfx_scene_shader_program.h
/// \author Peter Bartosch
///
/// \brief  pbj::gfx::SceneShaderProgram class header.

#ifndef GFX_SCENE_SHADER_PROGRAM_H_
#define GFX_SCENE_SHADER_PROGRAM_H_

#include <assert.h>
#include <iostream>
#include <map>
#include <string>
#include "pbj/_pbj.h"
#include "pbj/_math.h"
#include "pbj/gfx/shader.h"
#include "pbj/gfx/batcher.h"

#define BAD_GL_VALUE GLuint(-1)

using pbj::sw::ResourceId;
using std::map;
using std::string;

namespace pbj
{
namespace gfx
{
	struct Light
	{
		vec4 position;
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		F32 attenuation;
	};

	struct ShaderMaterial
	{
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		F32 shininess;
	};

	class SceneShaderProgram
	{
	public:
		static map<string,I32> nameToIndex;
		static map<I32,string> indexToName;

		SceneShaderProgram(const ResourceId&);
		~SceneShaderProgram();

		void init(const Shader&, const Shader&);
		void destroy();

		void setValues(const GLfloat*, const GLfloat*, const GLfloat*,
						const Light*, const ShaderMaterial*);
		const UniformConfig* getUniformConfigs(I32&); ///< Returns uniform configs for vars WITHOUT data
		void use();

		const ResourceId& getId() const;
		GLuint getGlId() const;

		const GLint& getProjectionMatLoc() const;
		const GLint& getModelviewMatLoc() const;
		const GLint& getMvpMatLoc() const;
		const GLint* getLightDataLocs() const;
		const GLint* getShaderMaterialLocs() const;


	private:
		static bool _mapMade;

		void checkLinkResult();

		ResourceId _resId;
		GLuint _glId;

		GLint _projectionMatLoc;
		GLint _modelviewMatLoc;
		GLint _mvpMatLoc;
		GLint _lightDataLocs[5];
		GLint _shaderMaterialLocs[4];

	};
} //namespace pbj::gfx
} //namespace pbj
#endif