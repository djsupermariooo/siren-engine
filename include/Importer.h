#ifndef SRN_IMPORTER_H
#define SRN_IMPORTER_H

#include <SRN_Platform.h>
#include <Model.h>
#include <vector>
#include <glm/glm.hpp>
#include <gl/glew.h>

using namespace std;
using namespace Siren;

class SRN_API Importer
{
public:
	Importer();
	~Importer();
	bool loadOBJ(const char* fileName, vector<glm::vec3> &out_vertices, vector<glm::vec2> &out_uvs, vector<glm::vec3> &out_normals);

};

#endif