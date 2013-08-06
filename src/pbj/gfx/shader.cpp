///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/shader.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::gfx::Shader functions

#include "pbj/gfx/shader.h"

#include <cassert>
#include <iostream>

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new shader object by uploading the shader source
///         to the GPU and compiling it.
///
/// \param  id Describes the location of the shader in the database it was
///         loaded from.
/// \param  type The type of shader: vertex or fragment.
/// \param  source A string containing the GLSL source code to compile.
Shader::Shader(const sw::ResourceId& id, Type type, const std::string& source)
    : type_(type),
      resource_id_(id),
      gl_id_(0)
{
    handle_.associate(this);

#ifdef PBJ_EDITOR
    setSource(source);
#endif

    compile_(source);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  destroys the shader object, ensuring that it is deleted from the
///         GPU.
Shader::~Shader()
{
    invalidate_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns a handle to this shader.
///
/// \return A Handle<Shader> referring to this shader object.
be::Handle<Shader> Shader::getHandle()
{
    return handle_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns a ConstHandle to this shader.
///
/// \return A ConstHandle<Shader> referring to this shader object.
const be::ConstHandle<Shader> Shader::getHandle() const
{
    return handle_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves this shader's ResourceId
///
/// \return A ResourceId defining the location of this shader in a database.
const sw::ResourceId& Shader::getId() const
{
    return resource_id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the type of this shader.
///
/// \return Type::TVertex or Type::TFragment.
Shader::Type Shader::getType() const
{
    return type_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the OpenGL ID of the compiled shader object.
///
/// \return the OpenGL shader object ID.
GLuint Shader::getGlId() const
{
    return gl_id_;
}

#ifdef PBJ_EDITOR
///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a shader without uploading it to the GPU or compiling.
///
/// \details only available in the editor.
Shader::Shader()
    : type_(TFragment),
      gl_id_(0)
{
    handle_.associate(this);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets this shader's name.
///
/// \details The shader name is not used outside the editor, but it also
///         determines the Id of the shader used to store it in a sandwich.
///
/// \param  name The new name for the shader.
void Shader::setName(const std::string& name)
{
    metadata_["__name__"] = name;
    resource_id_.resource = Id(name);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the shader's name.
/// 
/// \details The shader name is only available in the editor, as it is stored
///         as editor metadata.
///
/// \return The current name of the shader.
const std::string& Shader::getName() const
{
    return getMetadata("__name__");
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the sandwich where this shader should be stored.
///
/// \param  id the Id of the sandwich to store this shader in.
void Shader::setSandwich(const Id& id)
{
    resource_id_.sandwich = id;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets a metadata key-value pair.  (i.e. author, date, version, etc)
///
/// \param  key The metadata property to set.
/// \param  value The new value of the metadata property.
void Shader::setMetadata(const std::string& key, const std::string& value)
{
    if (key[0] == '_' && key[1] == '_')
    {
        PBJ_LOG(VNotice) << "Attempted to set invalid metadata property!" << PBJ_LOG_NL
                         << "   Sandwich ID: " << resource_id_.sandwich << PBJ_LOG_NL
                         << "     Shader ID: " << resource_id_.resource << PBJ_LOG_NL
                         << "  Metadata Key: " << key << PBJ_LOG_NL
                         << "Metadata Value: " << value << PBJ_LOG_END;

        throw std::invalid_argument("Metadata properties may not begin with '__'!");
    }

    if (value.length() == 0)
        metadata_.erase(key);
    else
        metadata_[key] = value;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a metadata value.
///
/// \param  key The metadata property to get.
///
/// \return The value of the requested property.
const std::string& Shader::getMetadata(const std::string& key) const
{
    auto i = metadata_.find(key);
    if (i != metadata_.end())
        return i->second;

    return nullString_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves all the metadata associated with this shader.
///
/// \return The  a const view of the metadata map associated with this
///         shader.
const std::map<std::string, std::string>& Shader::getMetadata() const
{
    return metadata_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the source for this shader.
///
/// \details If the shader has been compiled, it will be invalidated and
///         deleted.
///
/// \param  source The new source for the shader.
void Shader::setSource(const std::string& source)
{
    metadata_["__source__"] = source;
    metadata_["__infolog__"].clear();
    invalidate_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the source of this shader.
///
/// \return The source for the shader.
const std::string& Shader::getSource() const
{
    return getMetadata("__source__");
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the type of the shader. (vertex or fragment)
///
/// \param  type Either TVertex or TFragment.
void Shader::setType(Type type)
{
    if (type != type_)
    {
        type_ = type;
        invalidate_();
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Determines if the shader has been compiled successfully.
///
/// \return true if the shader is valid and can safely be used in programs.
bool Shader::isValid() const
{
    return gl_id_ != 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the output generated by the GPU when the shader was
///         compiled.
///
/// \details If the shader has not been compiled, returns an empty string.
///
/// \return The information log provided by the GPU describing any problems
///         found.
const std::string& Shader::getInfoLog() const
{
    return getMetadata("__infolog__");
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Compiles the shader.
///
/// \details compile() must be called after using setSource() in order to
///         compile the shader.
void Shader::compile()
{
    compile_(getSource());
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns a reference to a static, empty string object.
///
/// \return A reference to a static, empty string object.
std::string& Shader::nullString_() const
{
    static std::string null_str;
    return null_str;
}

#endif

///////////////////////////////////////////////////////////////////////////////
/// \brief  Helper for uploading source data to the GPU and compiling it.
///
/// \param  source The shader's source code.
void Shader::compile_(const std::string& source)
{
    invalidate_();

    GLuint gl_type;
    switch (type_)
    {
        case TVertex:
            gl_type = GL_VERTEX_SHADER;
            break;

        case TFragment:
            gl_type = GL_FRAGMENT_SHADER;
            break;

        default:
            assert(false);  // We are only dealing with vertex and fragment shaders
    }

    gl_id_ = glCreateShader(gl_type);

    const char* source_cstr = source.c_str();
    glShaderSource(gl_id_, 1, &source_cstr, NULL);
    glCompileShader(gl_id_);

    // check if there was a problem with compilation.
    GLint result = GL_FALSE;
    glGetShaderiv(gl_id_, GL_COMPILE_STATUS, &result);

    // if there was a problem, get the shader info log, display a warning, and throw an error.
    if (result != GL_TRUE)
    {
        GLint infolog_len;
        glGetShaderiv(gl_id_, GL_INFO_LOG_LENGTH, &infolog_len);
        char *infolog = new char[std::max(1, infolog_len)];
        glGetShaderInfoLog(gl_id_, infolog_len, NULL, infolog);

        PBJ_LOG(VError) << "Error compiling shader!" << PBJ_LOG_NL
                        << "      Sandwich ID: " << resource_id_.sandwich << PBJ_LOG_NL
                        << "        Shader ID: " << resource_id_.resource << PBJ_LOG_NL
                        << "      Shader Type: " << (type_ == TVertex ? "Vertex" : (type_ == TFragment ? "Fragment" : "Unknown")) << PBJ_LOG_NL
                        << "   GL Shader Type: " << gl_type << PBJ_LOG_NL
                        << "GL Compile Status: " << result << PBJ_LOG_NL
                        << "      GL Info Log: " << infolog << PBJ_LOG_NL
                        << "    Shader Source: " << source << PBJ_LOG_END;

#ifdef PBJ_EDITOR
        metadata_["__infolog__"] = infolog;
#endif
        delete[] infolog;

        invalidate_();
        throw std::runtime_error("Error compiling shader!");
    }
#ifdef PBJ_EDITOR
    else
    {
        // if this is the editor, we want to get the info log even if the compilation succeeded.
        GLint infolog_len;
        glGetShaderiv(gl_id_, GL_INFO_LOG_LENGTH, &infolog_len);
        char *infolog = new char[std::max(1, infolog_len)];
        glGetShaderInfoLog(gl_id_, infolog_len, NULL, infolog);

        metadata_["__infolog__"] = infolog;
        delete[] infolog;
    }
#endif
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Invalidates the shader, deleting it from GPU memory.
void Shader::invalidate_()
{
    if (gl_id_ != 0)
    {
        glDeleteShader(gl_id_);
        gl_id_ = 0;
    }
}

} // namespace pbj::gfx
} // namespace pbj
