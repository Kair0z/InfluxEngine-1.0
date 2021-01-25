#pragma once
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include "glm/mat2x2.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"

namespace Influx
{
	// Defining 'our' math structs

	// Vector
	using Vector2f = glm::vec2;
	using Vector2d = glm::dvec2;
	using Vector2u = glm::uvec2;
	using Vector2i = glm::ivec2;

	using Vector3f = glm::vec3;
	using Vector3d = glm::dvec3;
	using Vector3u = glm::uvec3;
	using Vector3i = glm::ivec3;

	using Vector4f = glm::vec4;
	using Vector4d = glm::dvec4;
	using Vector4u = glm::uvec4;
	using Vector4i = glm::ivec4;

	// Matrix
	using Matrix2x2 = glm::mat2x2;
	using Matrix3x3 = glm::mat3x3;
	using Matrix4x4 = glm::mat4x4;

	inline Matrix4x4 Identity4x4() { return Matrix4x4(1.0); }


	

	
}