///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/_math.h
/// \author Benjamin Crist
///
/// \brief  Includes GLM headers & defines typedefs.

#ifndef PBJ_MATH_H_
#define PBJ_MATH_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/epsilon.hpp>

/// \brief Convenience constant for converting angles
#define DEGTORAD 0.0174532925199432957f

/// \brief Convenience constant for converting angles
#define RADTODEG 57.295779513082320876f

namespace pbj {

typedef glm::ivec2 ivec2;  ///< 2-component integer-valued vector type.
typedef glm::ivec3 ivec3;  ///< 3-component integer-valued vector type.

typedef glm::vec2 vec2;    ///< 2-component float-valued vector type.
typedef glm::vec3 vec3;    ///< 3-component float-valued vector type.
typedef glm::vec4 vec4;    ///< 4-component float-valued vector type.
typedef glm::vec3 color3;  ///< 3-component float-valued vector type.  Used to represent an RGB color.
typedef glm::vec4 color4;  ///< 4-component float-valued vector type.  Used to represent an RGBA color.

typedef glm::quat quat;    ///< Quaternion type.  Used to represent an arbitrary 3D rotation.

typedef glm::mat3 mat3;    ///< 3x3 float-valued column-major matrix type.
typedef glm::mat4 mat4;    ///< 4x4 float-valued column-major matrix type.

} // namespace pbj

#endif
