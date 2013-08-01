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
#include "pbj/gfx/gfx_scene_shader_program.h"
#endif

using namespace pbj;
using namespace pbj::gfx;

map<string,I32> SceneShaderProgram::nameToIndex = map<string,I32>();
map<string,I32> SceneShaderProgram::indexToName = map<string,I32>();
bool SceneShaderProgram::_mapMade = false;

////////////////////////////////////////////////////////////////////////////////
/// \brief
/// \details
SceneShaderProgram::SceneShaderProgram(const ResourceId& resourceId) :
					_resId(resourceId), _glId(BAD_GL_VALUE)
{
	_projectionMatLoc = BAD_GL_VALUE;
	_modelviewMatLoc = BAD_GL_VALUE;
	_mvpMatLoc = BAD_GL_VALUE;
	for(int i=0;i<5;++i)
		_lightDataLocs[i] = BAD_GL_VALUE;
	for(int i=0;i<4;++i)
		_shaderMaterialLocs[i] = BAD_GL_VALUE;
	if(!_mapMade)
	{
		nameToIndex["projection_matrix"] = 0;
		nameToIndex["modelview_matrix"] = 1;
		nameToIndex["mvp_matrix"] = 2;
		nameToIndex["light.position"] = 3;
		nameToIndex["light.ambient"] = 4;
		nameToIndex["light.diffuse"] = 5;
		nameToIndex["light.specular"] = 6;
		nameToIndex["light.attenuation"] = 7;
		nameToIndex["material.ambient"] = 8;
		nameToIndex["material.diffuse"] = 9;
		nameToIndex["material.specular"] = 10;
		nameToIndex["material.shininess"] = 11;

		indexToName[0] = "projection_matrix";
		indexToName[1] = "modelview_matrix";
		indexToName[2] = "mvp_matrix";
		indexToName[3] = "light.position";
		indexToName[4] = "light.ambient";
		indexToName[5] = "light.diffuse";
		indexToName[6] = "light.specular";
		indexToName[7] = "light.attenuation";
		indexToName[8] = "material.ambient";
		indexToName[9] = "material.diffuse";
		indexToName[10] = "material.specular";
		indexToName[11] = "material.shininess";

		_mapMade = true;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \brief
/// \details
SceneShaderProgram::~SceneShaderProgram()
{
	if(_glId != BAD_GL_VALUE)
		destroy();
}

////////////////////////////////////////////////////////////////////////////////
/// \brief
/// \details
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

	//populate location variables
	_projectionMatLoc = glGetUniformLocation(_glId, (const GLchar*)"projection_matrix");
	_modelviewMatLoc = glGetUniformLocation(_glId, (const GLchar*)"modelview_matrix");
	_mvpMatLoc = glGetUniformLocation(_glId, (const GLchar*)"mvp_matrix");
	_lightDataLocs[0] = glGetUniformLocation(_glId, (const GLchar*)"light.position");
	_lightDataLocs[1] = glGetUniformLocation(_glId, (const GLchar*)"light.ambient");
	_lightDataLocs[2] = glGetUniformLocation(_glId, (const GLchar*)"light.diffuse");
	_lightDataLocs[3] = glGetUniformLocation(_glId, (const GLchar*)"light.specular");
	_lightDataLocs[4] = glGetUniformLocation(_glId, (const GLchar*)"light.attenuation");
	_shaderMaterialLocs[0] = glGetUniformLocation(_glId, (const GLchar*)"material.ambient");
	_shaderMaterialLocs[1] = glGetUniformLocation(_glId, (const GLchar*)"material.diffuse");
	_shaderMaterialLocs[2] = glGetUniformLocation(_glId, (const GLchar*)"material.specular");
	_shaderMaterialLocs[3] = glGetUniformLocation(_glId, (const GLchar*)"material.shininess");
}

////////////////////////////////////////////////////////////////////////////////
/// \brief	Deletes the shader programs
/// \details Deletes shader programs to free up the memory they have allocated
void SceneShaderProgram::destroy()
{
	if (_glId != BAD_GL_VALUE)
    {
        glDeleteProgram(_glId);
        _glId = BAD_GL_VALUE;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// \brief	Sets values paramaters passed in
/// \details sets the values of the matricies, lights, and materials
void SceneShaderProgram::setValues(const GLfloat* projectionMatrix,
								   const GLfloat* modelviewMatrix,
								   const GLfloat* mvpMatrix,
								   const Light* lightData,
								   const ShaderMaterial* shaderMaterialData)
{
	// matricies
	glUniformMatrix4fv(_projectionMatLoc, 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(_modelviewMatLoc, 1, GL_FALSE, modelviewMatrix);
	glUniformMatrix4fv(_mvpMatLoc, 1, GL_FALSE, mvpMatrix);

	// light data
	glUniform4fv(_lightDataLocs[0], 1, (GLfloat*)&lightData->position);
	glUniform4fv(_lightDataLocs[1], 1, (GLfloat*)&lightData->ambient);
	glUniform4fv(_lightDataLocs[2], 1, (GLfloat*)&lightData->diffuse);
	glUniform4fv(_lightDataLocs[3], 1, (GLfloat*)&lightData->specular);
	glUniform1f(_lightDataLocs[4], (GLfloat)lightData->attenuation);

	// materials
	glUniform4fv(_shaderMaterialLocs[0], 1, (GLfloat*)&shaderMaterialData->ambient);
	glUniform4fv(_shaderMaterialLocs[1], 1, (GLfloat*)&shaderMaterialData->diffuse);
	glUniform4fv(_shaderMaterialLocs[2], 1, (GLfloat*)&shaderMaterialData->specular);
	glUniform1f(_shaderMaterialLocs[3], (GLfloat)shaderMaterialData->shininess);
}

////////////////////////////////////////////////////////////////////////////////
/// \brief Return a \sa gfx::UniformConfig array that contains all
///        information for the shader EXCLUDING data.
/// \details The idea is that the data will be populated outside of this class,
///          so there is no need to do that here.  Additionally
///          \c UniformConfig.data is a \c void* so any data defined here could
///          be overwritten the next time the shader is accessed.
/// \param nUniforms[out] An I32 representing the number of uniforms in the
///        returned array.
/// \returns An array of \sa gfx::UniformConfig for each uniform in the shader.
const gfx::UniformConfig* SceneShaderProgram::getUniformConfigs(I32& nUniforms)
{
	nUniforms = 12;
	UniformConfig ret[12];
	ret[0].array_size = 1;
	ret[0].location = _projectionMatLoc;
	ret[0].type = UniformConfig::UM4f;

	ret[1].array_size = 1;
	ret[1].location = _modelviewMatLoc;
	ret[1].type = UniformConfig::UM4f;

	ret[2].array_size = 1;
	ret[2].location = _mvpMatLoc;
	ret[2].type = UniformConfig::UM4f;

	for(int i=3;i<8;++i)
	{
		ret[i].array_size = 1;
		ret[i].location = _lightDataLocs[i-3];
		ret[i].type = UniformConfig::U1f;
		if(i < 7)
		{
			ret[i+5].array_size = 1;
			ret[i+5].location = _shaderMaterialLocs[i-3];
			ret[i+5].type = UniformConfig::U1f;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \brief	Uses the shader on the geometry that follows
/// \details Grabs the shader program and uses the shader on the geometry that follows
void SceneShaderProgram::use()
{
	assert(_glId != BAD_GL_VALUE);
	glUseProgram(_glId);
}

////////////////////////////////////////////////////////////////////////////////
/// \brief	Returns the ID for the DB resource
/// \details
const ResourceId& SceneShaderProgram::getId() const
{
	return _resId;
}

////////////////////////////////////////////////////////////////////////////////
/// \briefeturns the ID for the shader program resource
/// \details
GLuint SceneShaderProgram::getGlId() const
{
	return _glId;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief
/// \details
const GLint& SceneShaderProgram::getProjectionMatLoc() const
{
	return _projectionMatLoc;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief
/// \details
const GLint& SceneShaderProgram::getModelviewMatLoc() const
{
	return _modelviewMatLoc;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief
/// \details
const GLint& SceneShaderProgram::getMvpMatLoc() const
{
	return _mvpMatLoc;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief
/// \details
const GLint* SceneShaderProgram::getLightDataLocs() const
{
	return _lightDataLocs;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief
/// \details
const GLint* SceneShaderProgram::getShaderMaterialLocs() const
{
	return _shaderMaterialLocs;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief
/// \details
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