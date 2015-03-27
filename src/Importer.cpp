#include <Importer.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace Siren;

	Importer::Importer()
	{
	}

	Importer::~Importer()
	{
	}

	bool Importer::loadOBJ(const char* fileName, vector<glm::vec3> &out_vertices, vector<glm::vec2> &out_uvs, vector<glm::vec3> &out_normals)
	{
		vector<unsigned int> vertexIndices, uvIndices, normalIndices;
		vector<glm::vec3> temp_vertices;
		vector<glm::vec2> temp_uvs;
		vector<glm::vec3> temp_normals;

		ifstream in(fileName, ios::in);
		if (!in)
		{
			cerr << "Cannot open " << fileName << endl;
			exit(1);
		}

		string line;
		while (getline(in, line))
		{
			if (line.substr(0, 2) == "v ")
			{
				istringstream s(line.substr(2));
				glm::vec3 v;
				s >> v.x;
				s >> v.y;
				s >> v.z;

				temp_vertices.push_back(v);
			}
			else if (line.substr(0, 2) == "f ")
			{
				istringstream s(line.substr(2));
				GLushort a, b, c;
				s >> a;
				s >> b;
				s >> c;
				a--;
				b--;
				c--;
				vertexIndices.push_back(a);
				vertexIndices.push_back(b);
				vertexIndices.push_back(c);
			}
			else if (line[0] == '#')
			{
			}
			else
			{
			}
		}

		for (unsigned int i = 0; i < vertexIndices.size(); i++)
		{
			unsigned int vertexIndex = vertexIndices[i];

			glm::vec3 vertex = temp_vertices[vertexIndex];

			out_vertices.push_back(vertex);
		}

		/*normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
		for (int i = 0; i < elements.size(); i += 3)
		{
			GLushort ia = elements[i];
			GLushort ib = elements[i + 1];
			GLushort ic = elements[i + 2];
			glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]), glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
			normals[ia] = normals[ib] = normals[ic] = normal;
		}*/

		return true;
	}
