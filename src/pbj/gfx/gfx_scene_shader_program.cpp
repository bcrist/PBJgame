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

////////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/gfx_scene_shader_program.cpp
/// \author Peter Bartosch
///
/// \brief  pbj::gfx::SceneShaderProgram class implementation.
#ifndef GFX_SCENE_SHADER_PROGRAM_H_
#include "pbj/gfx/scene_shader_program.h"
#endif

using namespace pbj;
using namespace pbj::gfx;

////////////////////////////////////////////////////////////////////////////////
SceneShaderProgram::SceneShaderProgram(const ResourceId& resourceId) :
					_resId(resourceId), _glId(BAD_GL_VALUE)
{
	specFromTexLoc = BAD_GL_VALUE;
	normalTextureLoc = BAD_GL_VALUE;
	projectionMatLoc = BAD_GL_VALUE;
	modelviewMatLoc = BAD_GL_VALUE;
	mvpMatLoc = BAD_GL_VALUE;
	normalMatLoc = BAD_GL_VALUE;
	for(int i=0;i<5;++i)
		lightDataLocs[i] = BAD_GL_VALUE;
	for(int i=0;i<4;++i)
		shaderMaterialLocs[i] = BAD_GL_VALUE;
}

////////////////////////////////////////////////////////////////////////////////
SceneShaderProgram::~SceneShaderProgram()
{
	if(_glId != BAD_GL_VALUE)
		destroy();
}

////////////////////////////////////////////////////////////////////////////////
void SceneShaderProgram::init(const Shader& vertShader, const Shader& fragShader)
{
#ifdef PBJ_DEBUG
    if (vertShader.getType() != Shader::TVertex)
        PBJ_LOG(VNotice) << "Expected first parameter to be vertex shader!" << PBJ_LOG_NL
                         << "       Sandwich ID: " << id.sandwich << PBJ_LOG_NL
                         << "        Program ID: " << id.resource << PBJ_LOG_NL
                         << "Shader Sandwich ID: " << vertex_shader.getId().sandwich << PBJ_LOG_NL
                         << "         Shader ID: " << vertex_shader.getId().resource << PBJ_LOG_END;

    if (fragShader.getType() != Shader::TFragment)
        PBJ_LOG(VNotice) << "Expected second parameter to be fragment shader!" << PBJ_LOG_NL
                         << "       Sandwich ID: " << id.sandwich << PBJ_LOG_NL
                         << "        Program ID: " << id.resource << PBJ_LOG_NL
                         << "Shader Sandwich ID: " << fragment_shader.getId().sandwich << PBJ_LOG_NL
                         << "         Shader ID: " << fragment_shader.getId().resource << PBJ_LOG_END;
#endif
	_glId = glCreateProgram();
    glAttachShader(_glId, vertShader.getGlId());
    glAttachShader(_glId, fragShader.getGlId());
    glLinkProgram(_glId);

    checkLinkResult();
}

////////////////////////////////////////////////////////////////////////////////
void SceneShaderProgram::destroy()
{
	if (_glId != BAD_GL_VALUE)
    {
        glDeleteProgram(_glId);
        _glId = BAD_GL_VALUE;
    }
}

////////////////////////////////////////////////////////////////////////////////
void SceneShaderProgram::setValues(const GLfloat* projectionMatrix,
								   const GLfloat* modelviewMatrix,
								   const GLfloat* mvpMatrix,
								   const GLfloat* normalMatrix,
								   const Light* lightData,
								   const ShaderMaterial* shaderMaterialData,
								   const bool specFromTexture)
{
	// matricies
	glUniformMatrix4fv(projectionMatLoc, 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(modelviewMatLoc, 1, GL_FALSE, modelviewMatrix);
	glUniformMatrix4fv(mvpMatLoc, 1, GL_FALSE, mvpMatrix);
	glUniformMatrix4fv(normalMatLoc, 1, GL_FALSE, normalMatrix);

	// light data
	glUniform4fv(lightDataLocs[0], 1, (GLfloat*)&lightData->position);
	glUniform4fv(lightDataLocs[1], 1, (GLfloat*)&lightData->ambient);
	glUniform4fv(lightDataLocs[2], 1, (GLfloat*)&lightData->diffuse);
	glUniform4fv(lightDataLocs[3], 1, (GLfloat*)&lightData->specular);
	glUniform1f(lightDataLocs[4], (GLfloat)lightData->attenuation);

	// materials
	glUniform1i(specFromTexLoc, specFromTexture);
	glUniform4fv(shaderMaterialLocs[0], 1, (GLfloat*)&shaderMaterialData->ambient);
	glUniform4fv(shaderMaterialLocs[1], 1, (GLfloat*)&shaderMaterialData->diffuse);
	glUniform4fv(shaderMaterialLocs[2], 1, (GLfloat*)&shaderMaterialData->specular);
	glUniform1f(shaderMaterialLocs[3], (GLfloat)shaderMaterialData->shininess);
}

////////////////////////////////////////////////////////////////////////////////
void SceneShaderProgram::use()
{
	assert(_glId != BAD_GL_VALUE);
	glUseProgram(_glId);
}

////////////////////////////////////////////////////////////////////////////////
const ResourceId& SceneShaderProgram::getId() const
{
	return _resId;
}

////////////////////////////////////////////////////////////////////////////////
GLuint SceneShaderProgram::getGlId() const
{
	return _glId;
}

////////////////////////////////////////////////////////////////////////////////
const GLint& SceneShaderProgram::getSpecFromTexLoc() const
{
	return specFromTexLoc;
}

////////////////////////////////////////////////////////////////////////////////
const GLint& SceneShaderProgram::getNormalTextureLoc() const
{
	return normalTextureLoc;
}

////////////////////////////////////////////////////////////////////////////////
const GLint& SceneShaderProgram::getProjectionMatLoc() const
{
	return projectionMatLoc;
}

////////////////////////////////////////////////////////////////////////////////
const GLint& SceneShaderProgram::getModelviewMatLoc() const
{
	return modelviewMatLoc;
}

////////////////////////////////////////////////////////////////////////////////
const GLint& SceneShaderProgram::getMvpMatLoc() const
{
	return mvpMatLoc;
}

////////////////////////////////////////////////////////////////////////////////
const GLint& SceneShaderProgram::getNormalMatLoc() const
{
	return normalMatLoc;
}

////////////////////////////////////////////////////////////////////////////////
const GLint* SceneShaderProgram::getLightDataLocs() const
{
	return lightDataLocs;
}

////////////////////////////////////////////////////////////////////////////////
const GLint* SceneShaderProgram::getShaderMaterialLocs() const
{
	return shaderMaterialLocs;
}

////////////////////////////////////////////////////////////////////////////////
void SceneShaderProgram::checkLinkResult()
{
	GLint result = GL_FALSE;
    glGetProgramiv(_glId, GL_LINK_STATUS, &result);

    if (result != GL_TRUE)
    {
        GLint infolog_len;
        glGetProgramiv(_glId, GL_INFO_LOG_LENGTH, &infolog_len);
        char *infolog = new char[std::max(1, infolog_len)];
        glGetProgramInfoLog(_glId, infolog_len, NULL, infolog);

        PBJ_LOG(VError) << "Error linking program!" << PBJ_LOG_NL
                        << "   Sandwich ID: " << _resId.sandwich << PBJ_LOG_NL
                        << "    Program ID: " << _resId.resource << PBJ_LOG_NL
                        << "GL Link Status: " << result << PBJ_LOG_NL
                        << "   GL Info Log: " << infolog << PBJ_LOG_END;

        delete[] infolog;

        destroy();
		//hmm don't know if I want it like this.  Whatevs
        throw std::runtime_error("Error linking program!");
    }
}