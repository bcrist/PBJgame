////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJgame\include\pbj\gfx\gfx_scene_shader_program.h
///
/// \brief	Declares the graphics scene shader program class.
////////////////////////////////////////////////////////////////////////////////
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
	////////////////////////////////////////////////////////////////////////////
	/// \struct	Light
	///
	/// \brief	A struct for representing light in a shader
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	////////////////////////////////////////////////////////////////////////////
	struct Light
	{
		vec4 position;
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		F32 attenuation;
	};

	////////////////////////////////////////////////////////////////////////////
	/// \struct	ShaderMaterial
	///
	/// \brief	A struct for representing the material of a shader.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	////////////////////////////////////////////////////////////////////////////
	struct ShaderMaterial
	{
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		F32 shininess;
	};

	////////////////////////////////////////////////////////////////////////////
	/// \class	SceneShaderProgram
	///
	/// \brief	Scene shader program.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	////////////////////////////////////////////////////////////////////////////
	class SceneShaderProgram
	{
	public:
		static map<string,I32> nameToIndex;
		static map<I32,string> indexToName;

		SceneShaderProgram();
		SceneShaderProgram(const ResourceId&);
		~SceneShaderProgram();

		void init(const Shader&, const Shader&);
		void destroy();

		void setValues(const GLfloat*, const GLfloat*, const GLfloat*,
						const Light*, const ShaderMaterial*);
		I32 getUniformConfigs(UniformConfig*); ///< Returns uniform configs for vars WITHOUT data
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