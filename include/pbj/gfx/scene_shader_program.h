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
#include "pbj/_pbj.h"
#include "pbj/_math.h"
#include "pbj/gfx/shader.h"

#define BAD_GL_VALUE GLuint(-1)

using pbj::sw::ResourceId;

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

	/*========================
	Material
	
		Contains data for materials to send to a shader
	========================*/
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
		SceneShaderProgram(const ResourceId&);
		~SceneShaderProgram();

		void init(const Shader&, const Shader&);
		void destroy();

		void setValues(const GLfloat*, const GLfloat*, const GLfloat*,
						const GLfloat*, const Light*, const ShaderMaterial*,
						const bool);
		void use();

		const ResourceId& getId() const;
		GLuint getGlId() const;

		//uniform variable loc accessors
		//including get for the sake of it, but I don't
		//think any accessors are really needed for these.
		//could be wrong.
		const GLint& getSpecFromTexLoc() const;
		const GLint& getNormalTextureLoc() const;
		const GLint& getProjectionMatLoc() const;
		const GLint& getModelviewMatLoc() const;
		const GLint& getMvpMatLoc() const;
		const GLint& getNormalMatLoc() const;
		const GLint* getLightDataLocs() const;
		const GLint* getShaderMaterialLocs() const;


	private:
		void checkLinkResult();

		ResourceId _resId;
		GLuint _glId;

		GLint specFromTexLoc;
		GLint normalTextureLoc;
		GLint projectionMatLoc;
		GLint modelviewMatLoc;
		GLint mvpMatLoc;
		GLint normalMatLoc;
		GLint lightDataLocs[5];
		GLint shaderMaterialLocs[4];

	};
} //namespace pbj::gfx
} //namespace pbj
#endif