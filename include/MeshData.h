#ifndef SRN_MESHDATA_H
#define SRN_MESHDATA_H

#include <SRN_Platform.h>

#include <glm/glm.hpp>

using namespace glm;

namespace Siren {

	struct SRN_API VertexData
	{
		vec3 position;
		vec3 normal;
		vec3 tangent;
		vec3 color;
		float U, V;
	};
}

#endif // SRN_MESHDATA_H